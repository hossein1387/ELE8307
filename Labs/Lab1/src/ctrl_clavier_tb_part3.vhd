LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;

--Entity declaration
ENTITY ctrl_clavier_tb IS
END ctrl_clavier_tb;

ARCHITECTURE tb OF ctrl_clavier_tb IS 

	-- Component Declaration for the Unit Under Test (UUT)
	component ctrl_clavier_bon is
		port(
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
			);
	end component;    
      
	--signal declaration
	signal ps2_clk :  std_logic := '1';
	signal ps2_dat :  std_logic;
	signal usedw   :  std_logic_vector(5 downto 0);
	signal key_code:  std_logic_vector(7 downto 0);		
	signal rd      :  std_logic;
	signal empty   :  std_logic;
    signal clk     :  std_logic := '1';
	signal rst_n   :  std_logic;

BEGIN

	--component instanciation
    U0_ctrl_clavier_bon : ctrl_clavier_bon
        port map    (ps2_clk 	=> ps2_clk,
		             ps2_dat 	=> ps2_dat,
		             usedw		=> usedw,
		             key_code 	=> key_code,
		             rd	 		=> rd,	
		             empty 		=> empty, 
		             clk		   => clk,
		             rst_n 		=> rst_n);
															
    clk <= not clk after 10000 ps;  
	ps2_clk <= not ps2_clk after 33300 ps;

	--This process controls every input to the design under test.
	do_check_out_result:process
    begin
        rst_n <= '0';
		rd <= '1';
		ps2_dat <= '0';  --start bit
		wait for 10 ns;
		rst_n <= '1';
		
        wait for 56600 ps;
        ps2_dat <= '1';  --bit 0
        wait for 66600 ps; 
        ps2_dat <= '0';  --bit 1
        wait for 66600 ps; 
        ps2_dat <= '1';  --bit 2
        wait for 66600 ps; 
        ps2_dat <= '0';  --bit 3
        wait for 66600 ps; 
        ps2_dat <= '1';  --bit 4
		wait for 66600 ps; 
        ps2_dat <= '0';  --bit 5
        wait for 66600 ps; 
        ps2_dat <= '1';  --bit 6
        wait for 66600 ps; 
        ps2_dat <= '1';  --bit 7		
        wait for 66600 ps; 
        ps2_dat <= '1';  --parite
        wait for 66600 ps; 
        ps2_dat <= '1';  --stop		
		
		
        wait for 10 ns;        
        wait;
    end process do_check_out_result;
  
end tb;