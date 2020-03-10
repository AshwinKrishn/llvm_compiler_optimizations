
////////////////////////////////////////////////////////////////////////////////

#ifndef __CLASSICAL_DATAFLOW_H__
#define __CLASSICAL_DATAFLOW_H__

#include <stdio.h>
#include <iostream>
#include <queue>
#include <vector>

#include "llvm/IR/Instructions.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/IR/CFG.h"

#define MAX_SIZE 4096
enum
{
    UNION,
    INTERSECTION
}meet_operator;

enum
{
    FORWARD,
    BACKWARD
}flow_direction;
namespace llvm {

// Add definitions (and code, depending on your strategy) for your dataflow
// abstraction here.
	/*
	 * Interface for meet operator
	 *
	 */
	class meet_op {
		virtual std::bitset<MAX_SIZE> meet(std::vector<std::bitset<MAX_SIZE>> input ) = 0;
		
	};
	/*
	 *union operator inheriting pure virtual meet_op
	 */
	
	class union_meet : public meet_op{
	    public:
		std::bitset<MAX_SIZE> result;    
		void union_op( std::bitset<MAX_SIZE> B)
		{
			result |= B ;
		}	
		std::bitset<MAX_SIZE> meet(std::vector<std::bitset<MAX_SIZE>> input) override
		{
			for(unsigned int i =0 ;i < input.size() ;i++)
			{
				union_op(input[i]);
			}
			return result;	
		}
	};
	
	/*
	 *intersection operator inheriting pure virtual meet_op
	 */
	
	class intersection_meet : public meet_op{

		std::bitset<MAX_SIZE> result;    
		void intersection_op( std::bitset<MAX_SIZE> B)
		{
			result &= B ;
		}	
		std::bitset<MAX_SIZE> meet(std::vector<std::bitset<MAX_SIZE>> input) override
		{
			for(unsigned int i =0 ;i < input.size() ;i++)
			{
				intersection_op(input[i]);
			}
			return result;	
		}
	};
	
}

#endif
