library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity decodeur7segment is
port (
	
	nombre_0a9 : in std_logic_vector(3 downto 0);
	segments : out std_logic_vector(6 downto 0)	
);
end entity decodeur7segment;

architecture rtl of decodeur7segment is
begin

	with( nombre_0a9 ) select
		segments <= 	"0111111" when "0000",
						"0000110" when "0001",
						"1011011" when "0010", 
						"1001111" when "0011", 
						"1100110" when "0100", 
						"1101101" when "0101", 
						"1111101" when "0110", 
						"0000111" when "0111", 
						"1111111" when "1000", 
						"1101111" when "1001", 
						"0000000" when others; 
	

end rtl;
	