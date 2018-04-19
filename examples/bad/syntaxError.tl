program
    var COUNT as int;
    var A as int;
    var B as int;
    var TEMP as int;
begin
    notakeyword; % This keyword does not exist in the language
    COUNT := readInt;
    if COUNT > 0 then
        A := 0;
        B := 1;
        while COUNT > 0 do
            writeInt A;
            TEMP := A + B;
            A := B;
            B := TEMP;
            COUNT := COUNT - 1;
        end;
    end;
end
