library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity ctrl_clavier_mauvais is
	port (
		ps2_clk 	: 	in std_logic;
		ps2_dat 	: 	in std_logic;
		
		usedw		:	out std_logic_vector(5 downto 0);
		key_code 	: 	out std_logic_vector(7 downto 0);
		rd	 		:	in std_logic;
		empty 		: 	out std_logic;
		
		clk			: 	in std_logic;
		rst_n 		: 	in std_logic
	);
end ctrl_clavier_mauvais; 



architecture rtl of ctrl_clavier_mauvais is
	
	signal s_rst : std_logic;
	signal s_fifo_wr : std_logic;
	signal s_fifo_in : std_logic_vector(7 downto 0);
	signal s_fifo_full : std_logic;
	
	signal ps2_bitcount : std_logic_vector(3 downto 0);
	signal ps2_word : std_logic_vector(10 downto 0);
	signal ps2_bitdone : std_logic;
	signal reg_fifo : std_logic_vector(7 downto 0);
	
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

begin

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
	s_fifo_in <= reg_fifo;
	s_rst <= not( rst_n );
	
	ACQ_PROCESS : process( clk, rst_n )
	begin
	
		if( rst_n = '0' ) then
	
			ps2_bitcount <= (others => '0');
			ps2_word <= (others => '0');
			ps2_bitdone <= '0';
	
		elsif( clk'event AND clk = '1' ) then
		
			if( ps2_clk = '0' ) then
		
				if( ps2_bitdone = '0' AND ps2_bitcount < 10) then
					ps2_bitdone <= '1';
					ps2_word <= ps2_dat & ps2_word(10 downto 1);
					ps2_bitcount <= ps2_bitcount + 1;
					s_fifo_wr <= '0';
				elsif( ps2_bitdone = '0' ) then
					ps2_bitdone <= '1';					
					reg_fifo <= ps2_word(9 downto 2);
					ps2_bitcount <= "0000";
					s_fifo_wr <= '1';
				end if;	
		
			else
				ps2_bitdone <= '0';
				s_fifo_wr <= '0';
			end if;		
		
		end if;	
	
	end process;
	
	
	
	

end rtl;