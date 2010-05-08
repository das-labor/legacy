#!/usr/bin/ruby
#
#

while(!$stdin.eof) do
line = $stdin.readline

if match = /[\s]*((T|IN|OUT)[0-9]{1,2})[\s]*=[\s]*((T|IN|OUT)[0-9]{1,2})[\s]*([|^&])[\s]*((T|IN|OUT)[0-9]{1,2})[\s]*;/.match(line)
	puts("    mov "+match[1]+", "+match[3])
	case match[5]
		when '|'
			conductor = "or  "
		when '^'
			conductor = "eor "
		when '&'
			conductor = "and "
		else
			fprintf($stderr,"ERROR")
	end
	puts("    "+conductor+match[1]+", "+match[6])

else
	puts(line)
end

end
