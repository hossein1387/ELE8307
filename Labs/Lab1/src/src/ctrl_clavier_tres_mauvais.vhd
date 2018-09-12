library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

	
entity ctrl_clavier_tres_mauvais is
	port (
		ps2_clk 	: 	in std_logic;
		ps2_dat 	: 	in std_logic;
		--
		usedw		:	out std_logic_vector(5 downto 0);
		key_code 	: 	out std_logic_vector(7 downto 0);
		rd	 		:	in std_logic;
		empty 		: 	out std_logic;
		--
		clk			: 	in std_logic;
		rst_n 		: 	in std_logic
		
		--in_ff		: in std_logic;
		--out_ff		: out std_logic
	);
end ctrl_clavier_tres_mauvais; 
------------------------------------------------------------------------
------------------------------------------------------------------------
architecture rtl of ctrl_clavier_tres_mauvais is
	
	signal s_rst : std_logic;
	signal s_fifo_wr : std_logic;
	signal s_fifo_in : std_logic_vector(7 downto 0);
	signal s_fifo_full : std_logic;
	--
	signal ps2_reg_decal : std_logic_vector(10 downto 0);
	signal ps2_reg_bitcount : std_logic_vector(3 downto 0);
	--
	signal fifo_reg_in	: std_logic_vector(7 downto 0);
		
	--
	signal ps2_bitcount : std_logic_vector(3 downto 0);
	signal ps2_word : std_logic_vector(10 downto 0);
	signal ps2_bitdone : std_logic;
	
	
------------------------------------------------------------------------	
	component lpm_fifo0 
	PORT
	(
		clock		: IN STD_LOGIC ;
		data		: IN STD_LOGIC_VECTOR (7 DOWNTO 0);
		rdreq		: IN STD_LOGIC ;
		wrreq		: IN STD_LOGIC ;
		empty		: OUT STD_LOGIC ;
		full		: OUT STD_LOGIC ;
		q			: OUT STD_LOGIC_VECTOR (7 DOWNTO 0);
		usedw		: OUT STD_LOGIC_VECTOR (5 DOWNTO 0);
		aclr		: IN STD_LOGIC
	);
	end component;
------------------------------------------------------------------------
begin
--
	u_fifo : lpm_fifo0
	port map (
		clock		=> clk,
		data		=> s_fifo_in,
		rdreq		=> rd,
		wrreq		=> s_fifo_wr,
		empty		=> empty,
		full		=> s_fifo_full,
		q			=> key_code,
		usedw		=> usedw,
		aclr		=> s_rst
	);
	
	s_fifo_in <= fifo_reg_in;
	s_rst <= not( rst_n );
	
	ACQ_PROCESS_PS2 : process ( ps2_clk, rst_n )
	begin
	
		if( rst_n = '0' ) then 
	
			ps2_reg_decal <= (others => '0');
			ps2_reg_bitcount <= (others => '0');			
			
		elsif( ps2_clk'event AND ps2_clk = '0' ) then
	
			ps2_reg_decal <= ps2_dat & ps2_reg_decal(10 downto 1);
			if( ps2_reg_bitcount < 11 ) then		
				ps2_reg_bitcount <= ps2_reg_bitcount + 1;
			else
				ps2_reg_bitcount <= "0001";
			end if;
	
		end if;
			
	end process ACQ_PROCESS_PS2;
	
		
	
	ACQ_PROCESS_FIFO : process( clk, rst_n, ps2_reg_bitcount, ps2_reg_decal )
	begin
	
		if( rst_n = '0' ) then
		
			fifo_reg_in <= (others => '0');
	
		elsif( clk'event AND clk = '1' ) then
			
			fifo_reg_in <= ps2_reg_decal(8 downto 1);
						
			if( ps2_reg_bitcount = 11 ) then						
				s_fifo_wr <= '1';
			else
				s_fifo_wr <= '0';
			end if;
		
		end if;	
	
	end process;
	

end rtl;
------------------------------------------------------------------------