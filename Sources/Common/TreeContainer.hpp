#ifndef TREECONTAINER_HPP
#define TREECONTAINER_HPP

#include "TreeNode.hpp"
#include "Types.hpp"
#include "Point3.hpp"
#include "IContainer.hpp"

namespace Common
{
	/**
	 * \brief World division tree container.
	 */
	template <typename ObjectType> class TreeContainer
	{
		TreeNode<ObjectType> 	*rootNode;
		Integer					cubeMaxSize;
		unsigned				density;

	
	public:
		IContainerNode<ObjectType> * getNodeAt(const Common::Point3<Integer> &) const;

		/**
		 * \brief ctor
		 * \param density [in] division density
		 * \param maxSize [in] farest point (x, y & z) in the "universe"
		 */
		TreeContainer(unsigned density, Integer maxSize);
	
		/**
		 * \brief dtor.
		 * Free memory.
		 */
		~TreeContainer();
	
		/**
		 * \brief Add object to tree at specified position.
		 */
		void add(const ObjectType, const Common::Point3<Integer> &);
	};

	// implementation

	template <typename ObjectType> 
	TreeContainer<ObjectType>::TreeContainer(unsigned density, Integer maxSize) : density (density), cubeMaxSize (maxSize)
	{
		// create whole tree
		rootNode = new TreeNode<ObjectType>(NULL, density);
	}

	template <typename ObjectType> 
	TreeContainer<ObjectType>::~TreeContainer()
	{
		delete rootNode;
	}

	template <typename ObjectType> 
	IContainerNode<ObjectType> * TreeContainer<ObjectType>::getNodeAt(const Common::Point3<Integer> &where) const
	{
		Common::Point3<Integer> half(cubeMaxSize >> 1, cubeMaxSize >> 1, cubeMaxSize >> 1);
		TreeNode<ObjectType> *position = rootNode;
	
		// find desired cube
		for (unsigned i = 0; i < density; i ++)
		{
			// does it make any sense? of course it will not work for first time
			if (where.getX() < half.getX() &&
				where.getY() < half.getY() &&
				where.getZ() < half.getZ())
			{
				position = position->getChild(0);
			}
			else if (where.getX() < half.getX() &&
					 where.getY() < half.getY() &&
					 where.getZ() >= half.getZ())
			{
				position = position->getChild(1);
				half.setZ(cubeMaxSize - ((cubeMaxSize - half.getZ()) >> 1));
			}
			else if (where.getX() >= half.getX() &&
					 where.getY() < half.getY() &&
					 where.getZ() < half.getZ())
			{
				position = position->getChild(2);
				half.setX(cubeMaxSize - ((cubeMaxSize - half.getX()) >> 1));
			}
			else if (where.getX() >= half.getX() &&
					 where.getY() < half.getY() &&
					 where.getZ() >= half.getZ())
			{
				position = position->getChild(3);
				half.setZ(cubeMaxSize - ((cubeMaxSize - half.getZ()) >> 1));
				half.setX(cubeMaxSize - ((cubeMaxSize - half.getX()) >> 1));
			}
			else if (where.getX() < half.getX() &&
					 where.getY() >= half.getY() &&
					 where.getZ() < half.getZ())
			{
				position = position->getChild(4);
				half.setY(cubeMaxSize - ((cubeMaxSize - half.getY()) >> 1));
			}
			else if (where.getX() < half.getX() &&
					 where.getY() >=half.getY() &&
					 where.getZ() >= half.getZ())
			{
				position = position->getChild(5);
				half.setY(cubeMaxSize - ((cubeMaxSize - half.getY()) >> 1));
				half.setZ(cubeMaxSize - ((cubeMaxSize - half.getZ()) >> 1));
			}
			else if (where.getX() >= half.getX() &&
					 where.getY() >= half.getY() &&
					 where.getZ() < half.getZ())
			{
				position = position->getChild(6);
				half.setY(cubeMaxSize - ((cubeMaxSize - half.getY()) >> 1));
				half.setX(cubeMaxSize - ((cubeMaxSize - half.getX()) >> 1));
			}
			else if (where.getX() >= half.getX() &&
					 where.getY() >= half.getY() &&
					 where.getZ() >= half.getZ())
			{
				position = position->getChild(7);
				half.setY(cubeMaxSize - ((cubeMaxSize - half.getY()) >> 1));
				half.setX(cubeMaxSize - ((cubeMaxSize - half.getX()) >> 1));
				half.setZ(cubeMaxSize - ((cubeMaxSize - half.getZ()) >> 1));
			}
		}

		return position;
	}

	template <typename ObjectType> 
	void TreeContainer<ObjectType>::add(const ObjectType what, const Common::Point3<Integer> &where)
	{
		getNodeAt(where)->addObject (what);
	}
}

#endif
