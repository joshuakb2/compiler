program
    var COUNT as int;
    var A as int;
    var B as int;
    var TEMP as int;
begin
    COUNT := readInt;
    if COUNT > 0 then
        A := 0;
        B := 1;
        while COUNT + 0 do % COUNT + 0 does not evaluate to boolean, so it cannot be a condition
            writeInt A;
            TEMP := A + B;
            A := B;
            B := TEMP;
            COUNT := COUNT - 1;
        end;
    end;
end
