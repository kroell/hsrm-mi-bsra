all-tests: cachesim
	./cachesim -a 32767 -c 65536 -g lin -l 256 -r 2  -w 1 >4.3a.txt
	./cachesim -a 65536 -c 65536 -g lin -l 256 -r 2  -w 1 >4.3b.txt
	./cachesim -a 131072 -c 65536 -g lin -l 256 -r 2  -w 1 >4.3c.txt
	./cachesim -a 81920 -c 65536 -g lin -l 256 -p lru -r 2 >4.4a.txt
	./cachesim -a 81920 -c 65536 -g lin -l 256 -p lfu -r 2 >4.4b.txt
	./cachesim -a 81920 -c 65536 -g lin -l 256 -p fifo -r 2 >4.4c.txt
	./cachesim -a 81920 -c 65536 -g lin -l 256 -p rand -r 2 >4.4d.txt
	./cachesim -c 1024 -g pat -l 16 -r 2 -p lru  >4.5a.txt
	./cachesim -c 1024 -g pat -l 16 -r 2 -p lru -w 1 >4.5b.txt
	./cachesim -c 1024 -g pat -l 16 -r 2 -p lru -w 2 >4.5c.txt
	./cachesim -c 1024 -g pat -l 16 -r 2 -p lru -w 4 >4.5d.txt


cachesim: