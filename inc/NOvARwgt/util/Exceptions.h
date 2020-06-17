/*
 * Exceptions.h:
 *  Custom internal exceptions.
 *
 *  Created on: Oct. 30, 2019
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */


#ifndef NOVARWGT_EXCEPTIONS_H
#define NOVARWGT_EXCEPTIONS_H

#include <stdexcept>

namespace novarwgt
{
	class UnsupportedGeneratorException : public std::runtime_error
	{
		public:
			// inherit constructor
			using std::runtime_error::runtime_error;
	};
}

#endif //NOVARWGT_EXCEPTIONS_H
