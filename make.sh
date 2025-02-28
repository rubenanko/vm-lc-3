if ! [ -d "build" ]; then
    mkdir build
fi

function COMPILE()
{
    gcc $1 -c build/${1:0:-1}.o
}

gcc main.c -o build/vm.exe