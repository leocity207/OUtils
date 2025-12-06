O::Zip
======

Overview
--------

.. doxygenfunction:: O::Zip

.. doxygenfunction:: O::Zip_Index

.. doxygenfunction:: O::Zip_Adjacent_Circular

.. doxygenfunction:: O::For_Each_Indexed

.. doxygenfunction:: O::For_Each_Zip



Basic Usage
-----------
Using O::Zip for for loop

.. code-block:: cpp

	std::vector<int> a = {1,2,3};
	std::vector<int> b = {3,1,0};

	for(auto&& [a_elt, b_elt] : O::Zip(a,b))
	{
		std::cout << a_elt + b_elt << std::endl;
	}

Using O::For_Each_Zip for for loop:

.. code-block:: cpp

	std::vector<int> a = {1,2,3};
	std::vector<int> b = {3,1,0};

	O::For_Each_Zip(a, b, [](int& a_elt, int& b_elt)
	{
		std::cout << a_elt + b_elt << std::endl;
	});

Using O::Zip_Index

.. code-block:: cpp

	std::vector<int> a = {1,2,3};

	for(auto&& [a_elt, index] : O::Zip_Index(a))
	{
		a_elt + index;
	}

Using O::For_Each_Indexed

.. code-block:: cpp

	std::vector<int> a = {1,2,3};

	O::For_Each_Indexed(a, [](int& a_elt, int index)
	{
		a_elt + index;
	});


Using O::Zip_Adjacent_Circular
.. code-block:: cpp

	std::vector<int> a = {1,2,3,2,1,-1,2};

	std::vector<int> mean_filter;
	for(auto&& [a_i, a_i_1] : O::Zip_Adjacent_Circular(a))
	{
		mean_filter= (a_i_1 - a_i)/2;
	}