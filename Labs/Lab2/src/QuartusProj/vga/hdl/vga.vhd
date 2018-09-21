Library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

--
-- Modified from original (Lab4 ver) by MAD, with 8 color model (32 intensity each) instead of 4x64
--

LIBRARY lpm; 
USE lpm.lpm_components.all; 

ENTITY vga is
   PORT(
	-- NIOSII side
	reset		: in std_logic;
	clk		    : in std_logic;
	addr		: in std_logic_vector(18 downto 0);
	data		: in std_logic_vector(31 downto 0);
	wr			: in std_logic;
	busy		: out std_logic;

	-- SRAM side
	SRAM_ADDR	: out std_logic_vector(17 downto 0);
	SRAM_CE_N	: out std_logic;
	SRAM_DQ		: inout std_logic_vector(15 downto 0);
	SRAM_LB_N	: out std_logic;
	SRAM_OE_N	: out std_logic;
	SRAM_UB_N	: out std_logic;
	SRAM_WE_N	: out std_logic;

	-- VGA side
	CLOCK_25	: in std_logic;
	VGA_BLANK	: out std_logic;
	VGA_B		: out std_logic_vector(9 downto 0);
	VGA_CLK		: out std_logic;
	VGA_G		: out std_logic_vector(9 downto 0);
	VGA_HS		: out std_logic;
	VGA_R		: out std_logic_vector(9 downto 0);
	VGA_SYNC	: out std_logic;
	VGA_VS		: out std_logic
	)
;
END vga;

ARCHITECTURE JPD of vga is
	TYPE sm1_type is (read, write1, write2);
	SIGNAL sm1 : sm1_type;
	SIGNAL current_color : STD_LOGIC_VECTOR(7 downto 0);

	SIGNAL start_read : STD_LOGIC;
	SIGNAL fifo_in_write, fifo_in_read, fifo_in_full, fifo_in_empty : STD_LOGIC;
	SIGNAL fifo_in_input, fifo_in_output : STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL latch_addr : STD_LOGIC_VECTOR(18 DOWNTO 0);
	SIGNAL latch_data : STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL delay_start : STD_LOGIC;

	SIGNAL nreset,sync_fifo_out : STD_LOGIC;
	SIGNAL fifo_out_write, fifo_out_read, fifo_out_full, fifo_out_empty : STD_LOGIC;
	SIGNAL fifo_out_input, fifo_out_output : STD_LOGIC_VECTOR(15 DOWNTO 0);
	SIGNAL fifo_out_usedw : STD_LOGIC_VECTOR(3 DOWNTO 0);

	SIGNAL latch_SRAM_ADDR : STD_LOGIC_VECTOR(17 DOWNTO 0);
	SIGNAL latch_SRAM_DQ_wr : STD_LOGIC_VECTOR(15 DOWNTO 0);
	SIGNAL latch_SRAM_DQ_rd : STD_LOGIC_VECTOR(15 DOWNTO 0);
	SIGNAL latch_SRAM_LB_N : STD_LOGIC;			
	SIGNAL latch_SRAM_UB_N : STD_LOGIC;
	SIGNAL latch_SRAM_OE_N : STD_LOGIC;
	SIGNAL latch_SRAM_WE_N : STD_LOGIC;
	SIGNAL latch_SRAM_DQ_in : STD_LOGIC;
	SIGNAL latch_fifo_out_write, latch_fifo_out_write_d1 : STD_LOGIC;

	SIGNAL video_on, video_on_v, video_on_h : STD_LOGIC;
	SIGNAL h_count, v_count : STD_LOGIC_VECTOR(9 DOWNTO 0);
	SIGNAL H_SYNC, V_SYNC : STD_LOGIC;

	SIGNAL write_address, read_address : STD_LOGIC_VECTOR(18 downto 0);
	SIGNAL write_data : STD_LOGIC_VECTOR(7 downto 0);

	-- COLOR INDEXES
	CONSTANT WHITE_COLORINDEX : 	std_logic_vector(2 downto 0) := "000";
	CONSTANT RED_COLORINDEX : 		std_logic_vector(2 downto 0) := "001";
	CONSTANT GREEN_COLORINDEX: 		std_logic_vector(2 downto 0) := "010";
	CONSTANT BLUE_COLORINDEX : 		std_logic_vector(2 downto 0) := "011";
	CONSTANT YELLOW_COLORINDEX : 	std_logic_vector(2 downto 0) := "100";
	CONSTANT BROWN_COLORINDEX : 	std_logic_vector(2 downto 0) := "101";
	CONSTANT CYAN_COLORINDEX : 		std_logic_vector(2 downto 0) := "110";
	CONSTANT PURPLE_COLORINDEX : 	std_logic_vector(2 downto 0) := "111";
	
	
BEGIN

FIFO_IN:	LPM_FIFO
	generic map (
		LPM_WIDTH => 32,
        LPM_WIDTHU => 7,
		LPM_NUMWORDS => 128,
        LPM_SHOWAHEAD => "ON"
	)

	port map (
		DATA => fifo_in_input,
		CLOCK => clk,
		WRREQ => fifo_in_write,
		RDREQ => fifo_in_read,
		ACLR => reset,
		Q => fifo_in_output,
        FULL => fifo_in_full,
        EMPTY => fifo_in_empty
	);
	
fifo_in_input <= "00000" & latch_data(7 downto 0) & latch_addr(18 downto 0);
fifo_in_write <= delay_start;
busy <= fifo_in_full;

process(clk,reset)
begin
	if (reset = '1') then
		delay_start <= '0';
		latch_addr <= (others => '0');
		latch_data <= (others => '0');
	elsif (clk'event and clk='1') then
		if (wr='1') then
			latch_addr <= addr;
			latch_data <= data;
		end if;
		delay_start <= wr or (delay_start and fifo_in_full); 
	end if;
end process;

FIFO_OUT:	LPM_FIFO_DC
	generic map (
		LPM_WIDTH => 16,
        LPM_WIDTHU => 4,
		LPM_NUMWORDS => 16
	)

	port map (
		DATA => fifo_out_input,
		WRCLOCK => clk,
		RDCLOCK => clock_25,
		WRREQ => fifo_out_write,
		RDREQ => fifo_out_read,
		ACLR => reset,
		Q => fifo_out_output,
		WRUSEDW => fifo_out_usedw,
        WRFULL => fifo_out_full,
        RDEMPTY => fifo_out_empty
	);
	
process (reset, clock_25)
    begin
	if (reset = '1') then nreset <= '0';
	elsif (clock_25'event and clock_25='1') then
		nreset <= '1';
	end if;
end process;

process (nreset,clock_25)
    begin
	if (nreset='0') then
		fifo_out_read <= '0';
		sync_fifo_out <= '0';
	elsif (clock_25'event and clock_25='1') then
		if ((v_count = 479) and (h_count = 638) and (fifo_out_empty = '0')) then
			sync_fifo_out <= '1';
		end if;
		if ( (v_count < 480) and (h_count < 640) and (h_count(0) = '0') ) then
	   		fifo_out_read <= sync_fifo_out;
		else 
			fifo_out_read <= '0';
		end if;
	end if;
end process;

current_color <= fifo_out_output(7 downto 0) when (h_count(0) = '0') else fifo_out_output(15 downto 8);

write_address <= fifo_in_output(18 downto 0);
write_data <= fifo_in_output(26 downto 19);

SRAM_CE_N <= '0';

process (clk)
begin
    if (clk'event and clk='1') then
		SRAM_ADDR <= latch_SRAM_ADDR;
		if (latch_SRAM_OE_N = '0') then SRAM_DQ <= (others => 'Z');
		else SRAM_DQ <= latch_SRAM_DQ_wr;
		end if;
		SRAM_LB_N <= latch_SRAM_LB_N;			
		SRAM_UB_N <= latch_SRAM_UB_N;
		SRAM_OE_N <= latch_SRAM_OE_N;
		latch_SRAM_DQ_rd <= SRAM_DQ;
	end if;
end process;
		

process (reset,clk)
	variable next_read_address : std_logic_vector(18 downto 0);
    begin
	if (reset='1') then
		sm1 <= read;
		latch_SRAM_ADDR <= (others => '0');
		latch_SRAM_DQ_wr <= (others => '0');
		latch_SRAM_LB_N <= '0';			
		latch_SRAM_UB_N <= '0';
		latch_SRAM_OE_N <= '0';
		latch_SRAM_WE_N <= '1';
		latch_fifo_out_write <= '0';

		read_address <= (others => '0');
		fifo_out_write <= '0';
		fifo_in_read <=	'0';

	elsif (clk'event and clk='1') then
		fifo_out_write <= latch_fifo_out_write;

		case sm1 is 
		when read =>
			if (fifo_out_usedw(3)='0') then
				if (read_address = 307198) then 
					next_read_address := (others => '0');
				else
					next_read_address := read_address + 2;
				end if;
				read_address <= next_read_address;
				latch_SRAM_ADDR <= next_read_address(18 downto 1);
				latch_fifo_out_write <= '1';
			else 
				latch_fifo_out_write <= '0';
				if (fifo_in_empty='0') then
					latch_SRAM_ADDR <= write_address(18 downto 1);
					latch_SRAM_DQ_wr <= write_data & write_data;
					latch_SRAM_LB_N <= write_address(0);			
					latch_SRAM_UB_N <= not write_address(0);
					latch_SRAM_OE_N <= '1';
					fifo_in_read <= '1';
					sm1 <= write1;
				end if;
			end if;
		
		when write1 =>
			latch_SRAM_WE_N <= '0';
			fifo_in_read <='0';
			sm1 <= write2;

		when write2 =>
			latch_SRAM_WE_N <= '1';
			if (fifo_out_usedw(3)='0') or (fifo_in_empty='1') then
				latch_SRAM_ADDR <= read_address(18 downto 1);
				latch_SRAM_DQ_wr <= (others => '0');
				latch_SRAM_LB_N <= '0';			
				latch_SRAM_UB_N <= '0';
				latch_SRAM_OE_N <= '0';
				sm1 <= read;
			else
				latch_SRAM_ADDR <= write_address(18 downto 1);
				latch_SRAM_DQ_wr <= write_data & write_data;
				latch_SRAM_LB_N <= write_address(0);			
				latch_SRAM_UB_N <= not write_address(0);
				fifo_in_read <= '1';
				sm1 <= write1;
			end if;				
		end case;
		
	end if;
end process;		

process (reset,clk)
    begin
	if (reset='1') then
		SRAM_WE_N <= '1';
	elsif (clk'event and clk='0') then
		SRAM_WE_N <= latch_SRAM_WE_N;
	end if;
end process;

fifo_out_input <= latch_SRAM_DQ_rd;

process (nreset,clock_25)
    begin
	if (nreset='0') then
	VGA_R <= (others => '0');
	VGA_G <= (others => '0');
	VGA_B <= (others => '0');

	elsif (clock_25'EVENT) AND (clock_25='1') then
		VGA_R <= (others => '0');
		VGA_G <= (others => '0');
		VGA_B <= (others => '0');
		if (video_on = '1') then
			case current_color(7 downto 5) is
				when WHITE_COLORINDEX => 
					VGA_R <= current_color(4 downto 0) & "00000";
					VGA_G <= current_color(4 downto 0) & "00000";
					VGA_B <= current_color(4 downto 0) & "00000";
				when RED_COLORINDEX => 
					VGA_R <= current_color(4 downto 0) & "00000";					
				when GREEN_COLORINDEX => 					
					VGA_G <= current_color(4 downto 0) & "00000";					
				when BLUE_COLORINDEX => 					
					VGA_B <= current_color(4 downto 0) & "00000";
				when YELLOW_COLORINDEX => 
					VGA_R <= current_color(4 downto 0) & "00000";
					VGA_G <= current_color(4 downto 0) & "00000";					
				when BROWN_COLORINDEX => 
					VGA_R <= "0" & current_color(4 downto 0) & "0000";
					VGA_G <= "00" & current_color(4 downto 0) & "000";
					VGA_B <= "0000" & current_color(4 downto 0) & "0";
				when CYAN_COLORINDEX => 					
					VGA_G <= current_color(4 downto 0) & "00000";
					VGA_B <= current_color(4 downto 0) & "00000";
				when PURPLE_COLORINDEX => 
					VGA_R <= current_color(4 downto 0) & "00000";					
					VGA_B <= current_color(4 downto 0) & "00000";	
			end case;
		end if;
	end if;
end process;

-- video_on is high only when RGB data is displayed
video_on <= video_on_H AND video_on_V;

process (nreset,clock_25)
    begin
	if (nreset='0') then
		H_SYNC <= '1';
		V_SYNC <= '1';
		h_count <= (others => '0');
		v_count <= "0111011111";
		video_on_h <= '1';
		video_on_v <= '1';
	elsif (clock_25'EVENT) AND (clock_25='1') then

--Generate Horizontal and Vertical Timing Signals for Video Signal
-- H_count counts pixels (640 + extra time for sync signals)
-- 
--  Horiz_sync  ------------------------------------__________--------
--  H_count       0                640             659       755    799
--
	IF (h_count = 799) THEN
   		h_count <= "0000000000";
	ELSE
   		h_count <= h_count + 1;
	END IF;

--Generate Horizontal Sync Signal using H_count
	IF (h_count <= 755) AND (h_count >= 659) THEN
 	  	H_SYNC <= '0';
	ELSE
 	  	H_SYNC <= '1';
	END IF;

--V_count counts rows of pixels (480 + extra time for sync signals)
--  
--  Vert_sync      -----------------------------------------------_______------------
--  V_count         0                                      480    493-494          524
--
	IF (v_count = 524) AND (h_count = 699) THEN
   		v_count <= "0000000000";
	ELSE
		IF (h_count = 699) THEN
	   		v_count <= v_count + 1;
		END IF;
	END IF;

-- Generate Vertical Sync Signal using V_count
	IF (v_count <= 494) AND (v_count >= 493) THEN
   		V_SYNC <= '0';
	ELSE
  		V_SYNC <= '1';
	END IF;

-- Generate Video on Screen Signals for Pixel Data
	IF (h_count = 799) THEN
   		video_on_h <= '1';
	ELSIF (h_count = 639) THEN
	   	video_on_h <= '0';
	END IF;

	IF (h_count = 699) THEN
		IF (v_count = 524) THEN
			video_on_v <= '1';
		ELSIF (v_count = 479) THEN
	   		video_on_v <= '0';
		END IF;
	END IF;

    END IF;
END PROCESS;

VGA_CLK <= not clock_25;
VGA_HS <= H_SYNC;
VGA_VS <= V_SYNC;
VGA_BLANK <= H_SYNC and V_SYNC;
VGA_SYNC <= '0';

END JPD;
