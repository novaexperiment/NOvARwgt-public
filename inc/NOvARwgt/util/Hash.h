/*
 * Hash.h:
 *  Tools to support making hashes for use in unordered_map<>s and unordered_set<>s.
 *
 *  Created on: Oct. 15, 2018
 *      Author: J. Wolcott <jwolcott@fnal.gov>
 */

#ifndef NOVARWGT_HASH_H
#define NOVARWGT_HASH_H

#include <functional>

namespace novarwgt
{
	namespace hash
	{
		/// Overload to handle when there are no variable arguments left
		inline std::size_t Hash(std::size_t seed = 0)
		{
			return seed;
		}

		/// Used for the hashing inside the weighter registry.
		/// magic values & idea adapted from here: https://stackoverflow.com/questions/23859844/hash-function-for-a-vector-of-pairint-int
		template <typename T>
		std::size_t Hash(const T& v, std::size_t seed)
		{
			seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
			return seed;
		}

		/// Specialization for char*.  Mysterious, bad things happen when you let char* go through the templated version above.
		/// (Somehow std::hash<>() appears to be mangling the memory inside the char* itself...)
		inline std::size_t Hash(const char * v, std::size_t seed)
		{
			return Hash(std::string(v), seed);
		}


		/// Hash function that recursively peels off arguments and adds them to the hash.
		/// Unfortunately you can't give an argument after a parameter pack, so the seed has to go first
		/// (the enable_if<> nonsense so that this version isn't ambiguous with the explicit two-argument version)
		template <typename T, typename ... Args>
		typename std::enable_if<!std::is_convertible<T,std::size_t>::value || sizeof...(Args) != 0, std::size_t>::type
		Hash(std::size_t seed, const T& t, Args && ... args)
		{
			seed = Hash(t, seed);
			return Hash(seed, std::forward<Args>(args)...);
		}
	}
}

#endif //NOVARWGT_HASH_H
