#define PI 3.1415926
function OtherDirection(d as integer) as integer
    d -= 2
    if d < 1 then d += 4
    if d > 4 then d -= 4
    return d
end function
