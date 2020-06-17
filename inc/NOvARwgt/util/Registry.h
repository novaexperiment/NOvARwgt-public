//
// Created by jeremy on 11/1/19.
//

#ifndef NOVARWGT_REGISTRY_H
#define NOVARWGT_REGISTRY_H

#include <memory>
#include <unordered_map>
#include <utility>

#include "NOvARwgt/util/Hash.h"

namespace novarwgt
{
	/// This namespace contains the inner workings of the registry.
	/// They're not meant for public use.
	namespace registry
	{
		/// Base class for the templated ClassID that'll be coming next.
		class _ClassID
		{
			public:
				explicit _ClassID(std::pair<unsigned long, unsigned long> id)
					: fId(std::move(id))
				{}

				const std::pair<unsigned long, unsigned long> & ID() const { return fId; }

				bool operator==(const _ClassID & other) const { return other.fId == this->fId; }
			private:
				std::pair<unsigned long, unsigned long> fId;
		};


		/// Make _ClassIDs hashable so they can be used as a key type in an unordered_map or unordered_set
		struct Hasher
		{
			unsigned long
			operator()(const _ClassID &id) const
			{ return novarwgt::hash::Hash(id.ID().first, id.ID().second); }
		};

	}

	/// Base class that allows us to put all registerable things in the same containers
	class IRegisterable
	{
		template<typename T, typename ... Args>
		friend const novarwgt::IRegisterable * GetRegisterable(Args && ... args);

		private:
			static unsigned int sIdCounter;   ///< counter that increments for unique classes

			/// Separate the major identifier (which only depends on the class) from the calculation based on the args.
			template<typename T>
			static unsigned long ClassMajorID()
			{
				// this is the class type ID
				static unsigned long counter = sIdCounter++;

				return counter;
			}

		protected:
			template <typename T>
			class ClassID : public registry::_ClassID
			{
				public:
					/// Combine class identifier from ClassMajorID()
					/// with sub-id from args (as necessary)
					/// to create a unique ID used to identify
					/// the singleton instance of the weighter class in the map
					template <typename ... Args>
					explicit ClassID(Args ... args)
						: _ClassID(std::make_pair(ClassMajorID<T>(), novarwgt::hash::Hash(0, std::forward<Args>(args)...)))
					{}
			};

		public:
			/// This constructor requires a ClassID object, which is a protected member type of this class.
			/// That way nothing but GetRegisterable() can call the constructor.
			/// (Idea adapted from http://seanmiddleditch.com/enabling-make-unique-with-private-constructors)
			template <typename T>
			explicit IRegisterable(const ClassID<T>&, std::string name)
			  : fName(std::move(name))
			{}

			virtual ~IRegisterable() = default;

			const std::string & Name() const { return fName; }

		private:
			const std::string fName;

	};

	//  --------------------
	//
	//  the following functions create a unique 'registry' for weight generators, knobs, etc.

	// put the inner workings inside their own namespace
	namespace registry
	{
		typedef std::unordered_map<registry::_ClassID, std::unique_ptr<novarwgt::IRegisterable>, registry::Hasher>
		        Registry;

		/// Internal function holding the registry.  Use GetRegisterable() or GetRegisterableByName() for the public interface.
		Registry & __GetRegistry();
	} // namespace registry

	/// semi-magic function which wraps up Registerable creation & retrieval so there's only ever one of each.
	// (for implementation of template, which is to be included in places it's used, see Registry.ixx.)
	template<typename T, typename ... Args>
	const novarwgt::IRegisterable * GetRegisterable(Args && ... args);

	/// obtain an already-existing registry entry by its name.
	/// will return nullptr if no such registry entry exists.
	/// caveats:
	///   * this process is slower than GetRegisterable()
	///   * there is no guarantee that different Registerables have different names,
	///     since the Registry keys are type & constructor arguments.  you might get one of several!
	/// so prefer GetRegisterable() if at all possible.
	const novarwgt::IRegisterable * GetRegisterableByName(const std::string & name);

}

#endif //NOVARWGT_REGISTRY_H
