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
#define EOBIN		2

typedef char name_t;
class profile;
class site_t;


typedef struct quartet {
	count_t base[5];
	bool masked;
	
	quartet (){
		masked=false;
		memset (base,0, 5*sizeof(count_t) );
	}
	
	quartet(const count_t &A, const count_t &C, const count_t &G, const count_t &T, const count_t &N) {
		base[0]=A;
		base[1]=C;
		base[2]=G;
		base[3]=T;
		base[4]=N;
	}

	quartet& operator+=(const quartet& x) {
		memcpy(base, x.base, 5*sizeof(count_t) );
		return *this;
	}
	inline quartet operator+(const quartet& x) const {
		return quartet(base[0]+x.base[0], base[1]+x.base[1], base[2]+x.base[2], base[3]+x.base[3], base[4]+x.base[4]);
	}
	quartet& operator=(const count_t& x) {
		memset (base, x, 5*sizeof(count_t) );
		return *this;
	}
} quartet_t;

count_t major(const quartet_t);
count_t minor(const quartet_t);
count_t error1(const quartet_t);
count_t error2(const quartet_t);

count_t count(const quartet_t);

class  profile_header{
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
	uint64_t *size_;			// the number of lines in the sample. 0 if unkown.

	char *delim_column;				// the delimiter that seperates columns.
	char *delim_quartet;				// the delimiter that seperates counts in a quartet.
	unsigned int *columns_;				// 5|6|7|?
	site_t *site_;					// a vector to store the calls from reads.
	unsigned int *samples_;				// the number of samples (i.e. different individuals or populations) in the profile.
public:
	const std::string getsample_property(const count_t &) const;
	const std::string getsample_name(const count_t &) const;
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

	

class site_t {
private:
public:
	count_t sorted_[5];				// an array to allow sorted access to quartets.
	site_t (count_t);
	site_t ();
	std::vector <quartet_t> sample;			//The five bases A/C/G/T/N;

	count_t samples_;

	count_t id0;
	uint64_t id1;				//The ids associated with the quartet.
	std::vector <count_t> extraid;		//extra ids associated with the quartet. (ref base identiy?).

	site_t & operator=(const site_t&);	

	const count_t getindex(count_t) const;		//returns the index of the alleles in order a sorted order
	count_t getcoverage(count_t) const;		//returns coverage of population/individual N
	count_t getcoverage(void) const;		//returns total coverage
	count_t getcount(count_t) const;		//returns the population count.
	count_t getcount(count_t, count_t) const;	//returns the count of individuals a's b'th allele.
	name_t getname(count_t) const;			//returns the name [*i.e. ACG or T] of the sorted alleles.
	name_t getname_gt(count_t) const;		//returns the name [*i.e. ACG or T] of the sorted alleles.
	void swap(count_t, count_t);			//exchage the alleles
	void sort(count_t);				//sort reads from most common to least common (based on poulation N).
	void sort(void);				//sort reads from most common to least common (amoung all non-masked sites).
	
	count_t maskedcount(void) const;		//returns the count of the number of individuals that are masked.
	const count_t *getquartet(count_t) const;	//returns the quartet array (unsorted)

	void maskall(void);				//mask all lines
	void unmask(count_t);				//unmask line N
	void unmask(quartet *);				//mask line N
	void mask(quartet *);				//mask line N
	void mask(count_t);				//mask line N


};

class  profile{
//private variables should be initialized by reading the header...
private:
	bool open_;					// indicates whether the profile opened succesfully

	/*these should all be controlled through the header_*/

	char delim_column;				// the delimiter which seperates columns
	char delim_quartet;				// the delimiter that seperates counts in a quartet
	unsigned int columns_;				// 5|6|7|more?
	unsigned int samples_;				// the number of samples (i.e. different individuals or populations) in the profile.
	uint64_t size_;					// the number of lines in the sample. 0 if unkown.
	bool sig_;					// were alleles thrown out if the allele only occurred in reads from one direction?
	bool read_;					// file is open for reading.
	bool write_;					// file is open for writing.
	bool binary_;					// binary mode flag. Incompatable with mpileup and noheader flags.
	bool mpileup_;					// file is an mpileup. Setting this should set the noheader flag.
	bool noheader_;					// file has no header

	/*done*/


	bool donothing_;				// a flag to indicate that nothing should be read for infile stream when read is called 

	static const count_t defaultorder[5];		// 01234 

	site_t site_;					// a structure that stores quartet information.

	int readm(site_t &);					//read file in mpileup mode.
	int readt(site_t &);					//read file in text mode.
	int readb(site_t &);					//read file in binary mode.

	int writet();					//write the quartet information in memory to file in text mode.
	int writeb();					//write the quartet information in memory to file in binary mode.
							//pro files do not contain enough information to construct mpileup files.

	int writet(site_t const &);			//write the quartet information passed to file in text mode.
	int writeb(site_t const &);			//write the quartet information passed to file in binary mode.

	profile_header header_;

	std::istream *in;				// all data is read from in.
	std::ostream *out;				// all data is writen is writen to out.
	std::fstream inFile;				// the file to read data from (if not stdin).
	std::ofstream outFile;				// the file to write data to (if not stdout).

	friend profile_header::profile_header(profile *);//Should set up pointers etc.
	friend void profile_header::init(profile *);	//Should set up pointers etc.
	void inline scan(const site_t &,const std::string &, quartet_t &); //?
public:
	profile();					//default constructor
	static const std::string names_;		// ACGTN

	profile* open(const char *, const char *);	//The function that opens a profile (if file).
	profile* open(const char *);			//The function that opens a profile (if stdin).
	bool is_open(void) const;			//returns true if profile is open, false otherwise.

	/*basic io operation*/
	int copy(const profile&);			//copys a line from profile
	int read();					//reads a line from the instream. Returns 0 on success, EOF on EOF.
	int read(site_t &);					//peaks at a line in the instream. Returns 0 on success, EOF on EOF.
	int write();					//writes a line to the outstream. Returns 0 on success, EOF on EOF.
	int write(site_t const &);			//writes a line to the outstream. Returns 0 on success, EOF on EOF.

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
	void set_delim_quartet(const char&);		// the delimiter which seperates columns

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
	uint64_t length(void) const {return size_;};	//number of lines in file

	/*functions dealing with the quartets*/


	void setbasecount(const count_t &, const count_t &, const count_t &);//sets sample x, base y to z.

	/*playing with the mask*/

	void mpileup(void) {mpileup_=true; noheader_=true;}	// file is an mpileup. Setting this should set the noheader flag.
	bool noheader(void) {noheader_=true;}			// file has no header

	const uint64_t getlinenumber(void) const;

	const count_t getid0(void) const;
	const uint64_t getid1(void) const;
	const count_t getextraid(const count_t &) const;

	void setid0(const count_t &);
	void setid1(const uint64_t &);
	void setextraid(const count_t &, const count_t &);

	std::string getids(const site_t &);
	std::string getids(void);

	void maskall(void);				//mask all lines
	void unmask(count_t);				//unmask line N
	void unmask(quartet *);				//mask line N
	void mask(quartet *);				//mask line N
	void mask(count_t);				//mask line N
	//THESE WILL PROBABLY BE DEPRICATED
	void sort(void);				//sort reads from most common to least common (amoung all non-masked sites).
	site_t getsite(void) {return site_;};		//sort reads from most common to least common (amoung all non-masked sites).

	std::vector <quartet_t>::iterator begin(void) {return site_.sample.begin();};	
	std::vector <quartet_t>::iterator end(void) {return site_.sample.end();};	

};
	
	
#endif
