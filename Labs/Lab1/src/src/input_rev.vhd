library IEEE;

use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity reverse_bits is
	port 
		(
			input_vec : in  std_logic_vector (7 downto 0);
			output_vec: out std_logic_vector (7 downto 0)
		);
end reverse_bits;

architecture Behavioral of reverse_bits is 

begin
	proc: process(input_vec) begin
		for i in 0 to 7 loop
				output_vec(i) <= input_vec(7-i);
		end loop;
	end process;
end Behavioral;
