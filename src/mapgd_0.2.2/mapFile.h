#ifndef PROFILE_H_
#define PROFILE_H_	

#include <cstdio>
#include <cstring>
#include <iostream>
#include <climits>
#include <stdlib.h> 	//Needed for one call to atoi

#include <list>
#include <vector>
#include <algorithm>
#include <map>
#include <string>

#include "typedef.h"
#include "streamtools.h"


#define NONE		0
#define BADHEADER	1
#define UNEXPECTED	2
#define VER		2.0

#define SKIP		1

#define NEWID0		1

typedef char name_t;
class profile;

class  mapfile_header{
private:
//private variables should be initialized by reading the header...
	std::map <std::string, count_t> id0_str;
	std::vector <std::string> id0;
	std::vector <std::string> extraids;		//extra ids associated with the quartet. (ref base identiy?).	

	std::vector <std::string> column_names;		// the names of all the columns in the profile.
	
	count_t lastid0;				//initilize to 0-1;
	std::string lastid0_str;			//initilize to "";

	count_t encodechar[256];
	std::string decodechar[256];


	bool *sig_;					// were alleles thrown out if the allele only occurred in reads from one direction?
	bool *read_;					// ?
	bool *write_;					// ?
	bool *binary_;					// ?
	bool *mpileup_;					// ?
	bool *noheader_;				// ?

	char *delim_column;				// the delimiter that seperates columns.
	char *delim_quartet;				// the delimiter that seperates counts in a quartet.
	unsigned int *columns_;				// 5|6|7|?
	unsigned int *samples_;				// the number of samples (i.e. different individuals or populations) in the profile.


public:
	char control;					//a variable that controls switches in the binary read/write mode.
	profile_header();				// does not initilize . . .
	profile_header(profile *);			// initilizes profile_header.
	void init(profile *);				// initilizes profile_header.

	const count_t encodeid0(const std::string &);
	const uint64_t encodeid1(const std::string &);
	const count_t encodeextraid(const char &, const count_t &);

	const std::string decodeid0(const count_t&);
	const std::string decodeid1(const uint64_t &);
	const std::string decodeextraid(const count_t &, const count_t &);

	int readheader(std::istream *);			// reads the header of a profile. All profiles from v 2.0 and later should have headers.
	int readtailer(std::istream *);			// reads the tailer of a profile. All profiles from v 2.0 and later should have headers.
	int writeheader(std::ostream *);		// write the . . .
	int writetailer(std::ostream *);		// write the . . .

	void set_delim_column(const char&);		// the delimiter which seperates columns
	void set_delim_quartet(const char&);		// the delimiter which seperates columns

	int setsamples(const count_t&);			//set the number of samples in the profile (only called in write mode).
	int setcolumns(const count_t&);			//set the number of columns for reading and writing.
	int setcolumn_name(const count_t&, const std::string&);
	const std::string getcolumn_name(const count_t&) const;
	profile_header & operator=(const profile_header&); //I don't think the copy makes sense. . . 
};

	
class  mapfile{
//private variables should be initialized by reading the header...
private:
	bool open_;					// indicates whether the profile opened succesfully

	/*these should all be controlled through the header_*/

	char delim_column;				// the delimiter which seperates columns
	char delim_genotype;				// the delimiter which seperates genotypes

	unsigned int columns_;				// 5|6|7|more?
	unsigned int samples_;				// the number of samples (i.e. different individuals or populations) in the profile.
	count_t size_;					// the number of lines in the sample. 0 if unkown.
	bool sig_;					// were alleles thrown out if the allele only occurred in reads from one direction?
	bool read_;					// file is open for reading.
	bool write_;					// file is open for writing.
	bool binary_;					// binary mode flag. Incompatable with mpileup and noheader flags.
	bool noheader_;					// file has no header

	/*done*/

	bool donothing_;				// a flag to indicate that nothing should be read for infile stream when read is called 

	static const std::string names_;		// ACGTN


	int readt(int);					//read file in text mode.
	int readb(int);					//read file in binary mode.

	int writet();					//write stat information in memory to file in text mode.
	int writeb();					//write stat information in memory to file in binary mode.


	profile_header header_;

	std::istream *in;				// all data is read from in.
	std::ostream *out;				// all data is writen is writen to out.
	std::fstream inFile;				// the file to read data from (if not stdin).
	std::ofstream outFile;				// the file to write data to (if not stdout).

	friend mapfile_header::mapfile_header(mapfile *);//Should set up pointers etc.
	friend void mapfile_header::init(mapfile *);	//Should set up pointers etc.

public:
	profile();					//default constructor

	profile* open(const char *, const char *);	//The function that opens a profile (if file).
	profile* open(const char *);			//The function that opens a profile (if stdin).
	bool is_open(void) const;			//returns true if profile is open, false otherwise.

	/*basic io operation*/
	int copy(const profile&);			//copys a line from profile
	int read();					//reads a line from the instream. Returns 0 on success, EOF on EOF.
	int read(int);					//peaks at a line in the instream. Returns 0 on success, EOF on EOF.
	int write();					//writes a line to the outstream. Returns 0 on success, EOF on EOF.

							//Both of these functions should thow an error if the are used while streams are not open.
	int seek(const std::streampos);			//goes to the pos streampos of the stream. Returns 0 on success, EOF on if streampos is not in the stream. 
	int seek(std::string, std::string);		//goes to the line specified by the ID0 ID1 pair om the instream. Returns 0 on success, EOF if ID0 ID1 is not in the stream. 

	void close(void);				//close iostreams, writes tail, etc.

	void static merge(std::list <profile *>);	//take multiple profile and combine them into a single profile.

	/*functions dealing with the header*/

	int copyheader(const profile&);			//copys the header of a profile.
	int readheader();				//reads the header of a profile. All profiles from v 2.0 and later should have headers.
	int writeheader();				//writes the header of a profile. All profiles from v 2.0 and later should have headers.

	void set_delim_column(const char&);		// the delimiter which seperates columns
	void set_delim_genotype(const char&);		// the delimiter which seperates columns

	int setsamples(count_t);			//set the number of samples in the profile (only called in write mode).
	int setcolumns(count_t);			//set the number of columns for reading and writing.

	int setcolumn_name(const count_t&, const std::string &);
	int setsample_name(const count_t&, const std::string &);

	const std::string getsample_name(const count_t &) const;

	/*a set of functions for converting the string information read in text mode into count_t and vice versa.*/

	const count_t encodeid0(const std::string &);
	const uint64_t encodeid1(const std::string &);
	const count_t encodeextraid(const char &, const count_t &);

	const std::string decodeid0(const count_t &);
	const std::string decodeid1(const uint64_t &);
	const std::string decodeextraid(const count_t &, const count_t &);

	/*functions dealing with ?*/
	count_t size(void) const;			//number of populations/individuals

	const count_t getindex(count_t) const;		//returns the index of the alleles in order a sorted order
	const count_t *getquartet(count_t) const;	//returns the quartet array (unsorted)

	std::string getids(void); 			//

	bool noheader(void) {noheader_=true;}			// file has no header

	const count_t getlinenumber(void) const;

	const count_t getid0(void) const;
	const uint64_t getid1(void) const;
	const count_t getextraid(const count_t &) const;

	void setid0(const count_t &);
	void setid1(const uint64_t &);
	void setextraid(const count_t &, const count_t &);
};
	
#endif
