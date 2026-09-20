cc -o sinegen sinegen.c -lm
echo "Type how many values you want in your table. More values means higher precision but a bigger table (index 0 is always sin(0), index __VALUE_YOU_TYPE__ is always sin(pi/2)): "
read granularity
./sinegen $granularity 15
cc -o benchmark -O0 benchmark.c -lm && ./benchmark
