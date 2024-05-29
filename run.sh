gcc main.c 
if [[ "$OSTYPE" == "msys" ]]; then
    ./a.exe "maze$1.txt" "$2"
        # Suppose the user use this for windows, its a lightweight shell and GNU utilities compiled for Windows (part of MinGW)
elif [[ "$OSTYPE" == "win32" ]]; then
    ./a.exe "maze$1.txt" "$2"
       # not sure of this but i least the first work, guess so
else
    ./a.out "maze$1.txt" "$2"
        # for the remaining, macos, linux...
fi