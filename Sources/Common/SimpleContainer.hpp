#ifndef COMMON_SIMPLECONTAINER_HPP
#define COMMON_SIMPLECONTAINER_HPP

#include "IContainer.hpp"
#include "SimpleNode.hpp"
#include "Types.hpp"
#include "Point3.hpp"
#include <cmath>

namespace Common
{
	/**
	 * \brief Simple non-tree container created for testing purposes. 
	 * It will be used as a reference for further optymalisations.
	 */
	template <class T>
	class SimpleContainer : public IContainer<T>
	{
		SimpleNode<T>	*nodes;

		//! Farest place at unit
		Integer			maxSize;

		//! Boxes count in each direction
		unsigned		boxesCount;

		//! Box size in internal units
		unsigned		boxSize;

	public:
		IContainerNode<T> * getNodeAt(const Point3<Integer> &pos) const
		{
			unsigned x, y, z;

			x = (unsigned)(pos.getX()/boxSize);
			y = (unsigned)(pos.getY()/boxSize);
			z = (unsigned)(pos.getZ()/boxSize);

			return &nodes[x*(boxesCount << 1) + y*boxesCount + z];
		}

	
		SimpleContainer(unsigned density, Integer maxSize) : maxSize (maxSize)
		{
			// density: 
			// 1 - 4 boxes [2x2x2] 
			// 2 - 64 boxes [4x4x4]
			// 3 - 512 boxes [8x8x8]
			// etc
			// count - 2^density
			// all   - count^3

			boxesCount = (unsigned)pow((float)2, (float)density);
			boxSize = (unsigned)ceil((float)maxSize/boxesCount);
			nodes = new SimpleNode<T>[boxesCount * boxesCount * boxesCount];
		}

		~SimpleContainer()
		{
			// free memory
			delete [] nodes;
		}

		void add(const T object, const Common::Point3<Integer> &point)
		{
			getNodeAt(point)->addObject(object);
		}
	};
}

#endif
