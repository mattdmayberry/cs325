## CS325-400-S2016 - Group 34: Project 4

### Team Members
Lauren Miller
 / Matt Mayberry
 / Collin Dorsett

### Files
	TSP.cpp
    tsp_example_1.txt
    tsp_example_2.txt
    tsp_example_3.txt
    tsp_example_1.txt.tour
    tsp_example_2.txt.tour
    tsp_example_3.txt.tour
    test-input-1.txt
    test-input-2.txt
    test-input-3.txt
    test-input-4.txt
    test-input-5.txt
    test-input-6.txt
    test-input-7.txt
    test-input-1.txt.tour
    test-input-2.txt.tour
    test-input-3.txt.tour
    test-input-4.txt.tour
    test-input-5.txt.tour
    test-input-6.txt.tour
    test-input-7.txt.tour
    README.md
    CS325_Group34_Project4T_TSP.pdf
    tsp-verifier.py
    TSPAllVisited.py
    TSPAllVisited.pyc
    
### Setup

To setup the program, simply clone the repository or download the project zip file, then follow the directions below.

	cd cs325-group34-project4
	g++ -o TSP TSP.cpp -lm

This will navigate you to the project directory and compile the program.

### Running the Program

Use the command below to run the program for any of the provided txt input files

	./TSP input_file.txt


### Output Files

Output files will be named with a `.tour` at the end of the file name.  

	$ ./TSP input_file.txt
	~~results~~
	$ ls
	TSP TSP.cpp input_file.txt input_file.txt.tour


### Running Tests
Testing is acheived using the provided `tsp-verifier.py` script. 

Use the command below to test output files

	python tsp-verifier.py example.txt example.txt.tour


### Using Git

See [Github's documentation](https://help.github.com/). 
