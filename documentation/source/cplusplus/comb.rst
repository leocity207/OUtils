O::Comb
===========

Overview
--------

.. doxygenclass:: O::Comb::View
	:members:
	:protected-members:
	:private-members:
	:undoc-members:

.. doxygenfunction:: O::Comv::Even

.. doxygenfunction:: O::Comv::Odd

.. doxygenfunction:: O::Comv::Make_Comb



Basic Usage
-----------

.. code-block:: cpp

	#include <utils/comb.h>

	std::vector<int> v{0,1,2,3,4,5,6,7};

	for(auto i : O::Comb::Even(v))
		std::cout << i; //0246