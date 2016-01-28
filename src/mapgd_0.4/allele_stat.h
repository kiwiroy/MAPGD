/* synonym for population? */

#ifndef ALLELE_STAT_H
#define ALLELE_STAT_H

#include <iostream>
#include "typedef.h"
#include "pro-file.h"
#include <cfloat>
#include <iomanip>

///	A class to store population specific information. May be moved over to population.
/** This is likely to become some form of container to handel moving data into and out of rows of map file.
 */
class allele_stat { 
private:
public:

	char delim;		//!< the delimiter used when reading/writing the class in text mode.	

	id0_t id0;		//!< the scaffold identifer of the allele.
	id1_t id1;		//!< the bp location of the allele.

	count_t excluded;	//!< A count of the number of samples that were excluded due to filtering criteria.
	bool pooled;		//!< Infered from pooled or labeled sequencing?

	allele_stat();		//

	float_t freq;		//!< frequency of major allele.

	gt_t ref;		//!< idenity of ref allele.
	gt_t minor;		//!< idenity of minor allele.
	gt_t major;		//!< idenity major allele.
	gt_t e1;		//!< idenity of error1
	gt_t e2;		//!< idenity of error2.

	float_t error;		//!< ml error rate.

	float_t null_error;	//!< error rate assuming monomorphism.
	float_t coverage;	//!< population coverage.
	float_t efc; 		//!< number of 'effective' chromosomes in the sample.

	float_t ll, monoll, hwell; //!< loglikelihoods.

	//FOR LABELED SEQUENCING ONLY!
	float_t MM; 		//!< frequency of genotype MM in the population.
	float_t Mm; 		//!< frequency of genotype Mm in the population.
	float_t mm; 		//!< frequency of genotype mm in the popualtion.

	float_t N;		//!< number of individual at the site.
	float_t f;		//!< HW statistic.
	float_t h;		//!< heterozygosity.

	float_t gof; 		//!< gof statistic.

	allele_stat & operator=(const allele_stat &);				//!< use the = operator to assign allele_stat.

	/**
	 * \defgroup basic_data
	 * @{
	 */

	allele_stat(std::vector<std::string>) : allele_stat(){};	//!< all data types need to 
	std::string header(void) const;		//!< names of columns.
	static const std::string table_name;	//!< The destination table in the Db.
	static const std::string file_name;	//!< DOOT!
//=".map";//!< The destination table in the Db.
	size_t size(void) const;	//!< The size of the ? in bytes.
	friend std::ostream& operator<< (std::ostream&, const allele_stat&);	//!< use the << operator to write allele_stat.
	friend std::istream& operator>> (std::istream&, allele_stat&);		//!< use the >> operator to read allele_stat.
	 // @}
};

#endif