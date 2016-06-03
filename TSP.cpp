#include <iostream> 
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
using namespace std;

//struct to hold information about a city's distance to another.
struct neighbor {
	int id; 
	int distance;
};

//holds results of TSP function
struct TSP_results {
	int* path;
	int distance;
	int cities_visited;
};

//takes name of input file, returns input file
FILE* open_input(char* fn_in) {
	FILE* f_in = fopen(fn_in, "r");
	if(f_in == NULL) {
		cout << "Error: cannot open input file\n";
		exit(0);
	}
	return f_in;
}

//takes name input file, returns output file
FILE* open_output(char* fn_in) {
	FILE* f_out;//the output file
	
	//appending ".tour" to the input filename, then opening the output file
	strcat(fn_in, ".tour");
	f_out = fopen(fn_in, "w+");
	if(f_out == NULL) {
		cout << "Error: cannot open output file\n";
		
		exit(0);
	}
	return f_out;
}

//takes the input file, returns the file as a string
char* get_file_text(FILE* f_in) {
	long f_size;//size of file
	char* f_text;//the text from the file
	
	fseek(f_in, 0, SEEK_END);
	f_size = ftell(f_in);
	rewind(f_in);

	f_text = (char*)malloc(f_size + 1);
	fread(f_text, f_size, 1, f_in);
	
	return f_text;	
}

//takes text with each city id, lat, and long on a line separated by spaces and returns how many cities there are
int get_city_count(char* f_text) {
	int city_count = 0;//the number of cities
	char* f_text_copy;//a copy of the text from the file, because strtok modifies it
	char* token;//a section of text from strtok
	
	f_text_copy = (char*)malloc(1 + strlen(f_text));
	strcpy(f_text_copy, f_text);
	
	token = strtok(f_text_copy, "\n\r");
	//counting the number of lines/cities
	while( token != NULL ) 
	{	
		city_count++;
		token = strtok(NULL, "\n\r");
	}
	
	free(f_text_copy);//freeing memory
	
	return city_count;
}

//takes the number of cities and text with each city id, lat, and long on a line separated by spaces, then returns a 2D array with those values copied in, where the first index indicates the given line from the input file and the second index indicates numbers 1-3 from that line
int** get_city_table(int city_count, char* f_text) {
	char* f_text_copy;//a copy of the text from the file, because strtok modifies it
	char* token;//section of input file from strtok
	char* token2;//section of input file from strtok
	char *strtok_ptr;//pointer for re-entrant strtok
	int** city_table;//table to hold the city information
	int i;
	
	f_text_copy = (char*)malloc(1 + strlen(f_text));
	strcpy(f_text_copy, f_text);
	city_table = (int**)malloc((city_count)*sizeof(int*));
	for(i = 0; i < city_count; i++) {
		city_table[i] = (int*)malloc(3*sizeof(int));
	}
	
	//adding the values to city_table
	city_count = 0;
	token = strtok_r(f_text_copy, "\n\r", &strtok_ptr);
	while(token != NULL) {//getting each line
		token2 = strtok(token, " ");
		city_table[city_count][0] = atoi(token2);
		
		token2 = strtok(NULL, " ");
		city_table[city_count][1] = atoi(token2);
		
		token2 = strtok(NULL, " ");
		city_table[city_count][2] = atoi(token2);
		
		token = strtok_r(NULL, "\n\r", &strtok_ptr);
		city_count++;
	}
	
	free(f_text_copy);//freeing memory
	
	return city_table;	
}

//takes number of cities and city table, the 2D array of city ids, lats, and longs, then returns a table with ids and distances to each neighbor
struct neighbor** get_city_distances_table(int city_count, int** city_table) {
	struct neighbor** city_distances_table;//table to hold the city information
	int i, j;
	int passed_city = 0;//0 if the city is figuring distances with ids less than its own, -1 if doing so for those with ids greater than its own
	double distance;
	
	//malloc for city distances table
	city_distances_table = (struct neighbor**)malloc(city_count*sizeof(struct neighbor*));
	for(i = 0; i < city_count; i++) {
		city_distances_table[i] = (struct neighbor*)malloc((city_count - 1)*sizeof(struct neighbor));
	}

	for(i = 0; i < city_count; i++) {//for every city, copy over the id of every other city and the distance to it
		j = 0;
		passed_city = 0;
		
		while(j < city_count) {
			//if j == i, continuing - not copying over the distance to this value, and setting passed_city to -1
			if(i == j) {
				passed_city = -1;
			}
			else {
				
				//copying over the id, factoring passed_city
				city_distances_table[i][j + passed_city].id = city_table[j][0];
				
				//figuring distance, copying over
				distance = round(sqrt(pow((city_table[i][1] - city_table[j][1]),2) + pow((city_table[i][2] - city_table[j][2]),2)));
				city_distances_table[i][j + passed_city].distance = (int)distance;
				
			}

			j++;
		}
	}
	
	return city_distances_table;
}

//takes the city table and city count, then deallocates memory 
void free_city_table(int** city_table, int city_count) {
	int i;
	for(i = 0; i < city_count; i++) {
		free(city_table[i]);
	}
	free(city_table);
}

//takes the city distances table and city count, then deallocates memory 
void free_city_distances_table(struct neighbor** city_distances_table, int city_count) {
	int i;
	for(i = 0; i < city_count; i++) {
		free(city_distances_table[i]);
	}
	free(city_distances_table);
}

//takes the TSP_results, then deallocates memory 
void free_TSP_results(struct TSP_results* results) {
	free(results->path);
	free(results);
}

//takes the output file, the city table, array of city ids as they are visited, and the number of cities, then prints the results for the TSP problem to the output file
void print_results(FILE* f_out, struct TSP_results* results, int city_count) {
	int i;
	fprintf(f_out, "%i\n", results->distance);
	printf("%i\n", results->distance);//LAUREN, remove later
	
	for(i = 0; i < city_count; i++) {
		fprintf(f_out, "%i\n", results->path[i]);
		printf("%i\n", results->path[i]);//LAUREN, remove later
	}
}

//takes the city_distances_table and the nummber of cities, returns a TSP_struct with the path, (the indexes of cities as they're visited), the total distance traveled, and the number of citites visited
struct TSP_results* nearest_neighbor(struct neighbor** city_distances_table, int city_count, int starting_city) {
	
	const int max_distance = 2147483647;// the assumed max distance from one city to another, what is typically an int max
	bool all_neighbors_visited = false; //tracks whether there are unvisited neighbors for the current_city
	int current_city; //the id of the city visited at a given point
	bool visited_table[city_count]; //a table to hold the value of whether a city with the id of the array's index has been visited yet
	int min_distance_neighbor;//the id of the neighbor the closest to current_city
	int min_distance;//the minimum distance to the nearest neighbor to current_city
	int i;
	
	struct TSP_results* results = (struct TSP_results*)malloc(sizeof(struct TSP_results*)); //the struct to hold the results for the TSP
	results->path = (int*)malloc((1 + city_count)*sizeof(int));//the indexes of cities as they are visited
	results->distance = 0; //the total distance between cities visited
	results->cities_visited = 0; //the total number of cities visited
	
	//initializing visited_table to false
	for(i = 0; i < city_count; i++) {
		visited_table[i] = false;
	}
	
	//select starting city/ current_city
	current_city = starting_city;
	results->path[0] = current_city;
	results->cities_visited++;
	
	
	//inside while loop of all_neighbors_visited == false, while current_city has unvisited neighbors
	while(!all_neighbors_visited) {
		visited_table[current_city] = true; //visiting the current_city
		all_neighbors_visited = true;//assuming there are no unvisited neighbors until proven otherwise
		min_distance = max_distance;
		
		//visiting all neighbors
		for(i = 0; i < city_count - 1; i++) {
			if(!visited_table[city_distances_table[current_city][i].id]) {//if the neighbor hasn't been visited
				if(city_distances_table[current_city][i].distance < min_distance) {//if the neighbor is closer than the nearest neighbor so far
					min_distance = city_distances_table[current_city][i].distance;
					min_distance_neighbor = city_distances_table[current_city][i].id;
					
					all_neighbors_visited = false;
				}
				
			}
		}
		
		if(!all_neighbors_visited) {//if there were unvisited neighbors
			
			//adding the current one to the path
			results->path[results->cities_visited] = min_distance_neighbor;
			results->distance += min_distance;
			results->cities_visited++;
			
			current_city = min_distance_neighbor;//going to the next city
		}
	}
	
	for(i = 0; i < city_count - 1; i++) {//going through neighbors of last city visited to get distance to starting_city
		if(city_distances_table[current_city][i].id == starting_city) {
			results->distance += city_distances_table[current_city][i].distance; 
		}
	}
	results->path[city_count] = starting_city;

	return results;
}


int main(int argc, char* argv[]){
	FILE* f_in;//the file read
	FILE* f_out;//the file written to
	char* f_text;//the text from the file
	int city_count;//the number of cities
	int** city_table;//table to hold the city information
	struct neighbor** city_distances_table;//table to hold the city information
	clock_t begin, end;//used to measure running time
	double run;//run time length
	double run_once;//run time length for getting one TSP results
	struct TSP_results* results;//the struct that holds the results from nearest neighbor
	struct TSP_results* results2;//the struct that holds the results from nearest neighbor with a different starting point
	int starting_city = 0;//the city where the TSP starts
	
	begin = clock();//starting timer
	
	//checking if user has provided filename
	if(argc != 2){
		cout << "Error: incorrect number of arguments\n";
		cout << "Usage: TSP [filename]\n";
		exit(0);
	}
	
	//opening input and output
	f_in = open_input(argv[1]);
	f_out = open_output(argv[1]);
	
	//getting the text from the file
	f_text = get_file_text(f_in);
	fclose(f_in);
	
	//getting the number of cities
	city_count = get_city_count(f_text);
	if(city_count == 0) {
		cout << "Error: no cities\n";
		exit(0);
	}
	
	//getting the city table
	city_table = get_city_table(city_count, f_text);

	//getting the table with distances for each city to every other city
	city_distances_table = get_city_distances_table(city_count, city_table);

	//getting the total distances and path for TSP
	results = nearest_neighbor(city_distances_table, city_count, starting_city); 
	
	
	end = clock();//ending timer
	run_once = 10 + (double)(end - begin) / CLOCKS_PER_SEC;//the time it took to run get the results once, with an extra buffer of 10 seconds
	run = (double)(end - begin) / CLOCKS_PER_SEC;
	
	while(180 - run > run_once && starting_city < city_count - 1) {//while there is still time available (for a 180 seconds max), try nearest neighbor with a new starting point, to see if we can get better distance
		starting_city++;
		results2 = nearest_neighbor(city_distances_table, city_count, starting_city);
		
		if(results->distance <= results2->distance) {//if results is better than results2
			free_TSP_results(results2);
		}
		else {//if results2 is better than results
			free_TSP_results(results);
			results = results2;
			results->path = results2->path;
		}
		
		end = clock();//ending timer
		run = (double)(end - begin) / CLOCKS_PER_SEC;
	}
	
	//cout << "Last starting city: " << starting_city << endl;//LAUREN
	
	
	
	//writing out the TSP results to f_out
	print_results(f_out, results, city_count);
	
	//closing the output file
	fclose(f_out);	
	
	//freeing memory allocated using malloc
	free(f_text);//f_text memory
	free_city_table(city_table, city_count);//city_table memory
	free_city_distances_table(city_distances_table, city_count);//city_distances_table
	free_TSP_results(results);//TSP_results
	
	end = clock();//ending timer
	run = (double)(end - begin) / CLOCKS_PER_SEC;
	cout << "Running Time: " << run << endl;
	
	return 0;
}