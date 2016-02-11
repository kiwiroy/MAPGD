/* synonym for population? */

#ifndef RELATEDNESS_DATA_H_
#define RELATEDNESS_DATA_H_

#include <iostream>
#include <cfloat>
#include <iomanip>
#include <vector>
#include <sstream>

#include "typedef.h"
#include "data.h"

/// A class to store population specific information. May be moved over to population.
/*  This is likely to become some form of container to handel moving data into 
 *  and out of rows of map file.
 */
class Relatedness : public Data{ 
private:
	void write (std::ostream&) const;	//!< use to write Allele. Inherits <<
	void read (std::istream&);		//!< use to read Allele. Inherits >>
	static const Registration registered;
	static Data * create(const std::vector <std::string> & Columns){
		return new Relatedness(Columns);
	}
public:
public:
	char delim;	//!< the delimiter used when reading/writing the class in text mode.	

	std::string X_;	//!< the name of the first (X) sample in the compairison.
	std::string Y_;	//!< the name of the second (Y) sample in the compairison.

	id1_t sites;	//!< the number of sites analyzed.

	float_t e_X_[8], e_X_ll;
	float_t e_Y_[8], e_Y_ll;
	float_t f_X_, f_X_ll;
	float_t f_Y_, f_Y_ll;
	float_t theta_XY_, theta_XY_ll;
	float_t gamma_XY_, gamma_XY_ll;
	float_t gamma_YX_, gamma_YX_ll;
	float_t delta_XY_, delta_XY_ll;
	float_t Delta_XY_, Delta_XY_ll;
	float_t null_ll_, max_ll_;

	Relatedness();	
	//! delegating a neccisary constructor.	
	Relatedness(const std::vector <std::string> &) : Relatedness(){}; 
	//! construct with names. 
	Relatedness(const std::string &, const std::string &);		  

	//! The header line of plain text files. 
	std::string header(void) const;
	//! Size in bytes for binary read/write. 
	size_t size(void) const;

	//! Sets the name of the X sample. 
	void set_X_name(const std::string &);
	//! Sets the name of the Y sample. 
	void set_Y_name(const std::string &);

	//! zeros statistics and sets names to empty.
	void clear(void); 
	//! zeros statistics, but doesn't set names to empty.
	void zero(void);  

	static const std::string file_name;	//!< The dafualt extention for files.
	static const std::string table_name;	//!< Destination table in Db.
};

#endif
