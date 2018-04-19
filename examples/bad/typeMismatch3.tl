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
        while COUNT > 0 do
            writeInt A;
            TEMP := A + B;
            A := B;
            B := TEMP - (A > B); % TEMP is of type INT and (A > B) is of type BOOL, but the minus operator requires them both to be of type INT
            COUNT := COUNT - 1;
        end;
    end;
end
