library IEEE;

use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity decodeur_segments is
	port 
		(
			dec_in    : in  std_logic_vector (3 downto 0);
			seven_seg : out std_logic_vector (6 downto 0)		
		);
end decodeur_segments;

architecture Behavioral of decodeur_segments is 

begin
	proc: process(dec_in) begin
		case dec_in is
			when "0000" => seven_seg <= "1000000"; ---0
			when "0001" => seven_seg <= "1111001"; ---1
			when "0010" => seven_seg <= "0100100"; ---2
			when "0011" => seven_seg <= "0110000"; ---3
			when "0100" => seven_seg <= "0011001"; ---4
			when "0101" => seven_seg <= "0010010"; ---5
			when "0110" => seven_seg <= "0000010"; ---6
			when "0111" => seven_seg <= "1111000"; ---7
			when "1000" => seven_seg <= "0000000"; ---8
			when "1001" => seven_seg <= "0010000"; ---9
			when others => seven_seg <= "1111111"; ---null
			end case;
	end process;
end Behavioral;
