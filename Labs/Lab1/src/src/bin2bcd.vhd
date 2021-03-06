library IEEE;

use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity bin_to_bcd is
	port 
		(
			input_bin : in  std_logic_vector (7 downto 0);
			hundreds  : out std_logic_vector (3 downto 0);
			tens      : out std_logic_vector (3 downto 0);
			ones      : out std_logic_vector (3 downto 0)		
		);
end bin_to_bcd;

architecture Behavioral of bin_to_bcd is 

begin
	proc: process(input_bin)
		variable shift_val : unsigned(19 downto 0);
		alias num is shift_val(7 downto 0)   ;
		alias one is shift_val(11 downto 8)  ;
		alias ten is shift_val(15 downto 12) ;
		alias hun is shift_val(19 downto 16) ;
		begin
			num := unsigned(input_bin);
			one := X"0";
			ten := X"0";
			hun := X"0";
			
			for i in 1 to num'LENGTH loop
				if one >= 5 then
					one := one +3;
				end if;
				if ten >= 5 then
					ten := ten +3;
				end if;
				if hun >= 5 then
					hun := hun +3;
				end if;
				shift_val := shift_left(shift_val, 1);
			end loop;
			
			hundreds <= std_logic_vector(hun);
			tens     <= std_logic_vector(ten);
			ones     <= std_logic_vector(one);
	end process;
end Behavioral;
