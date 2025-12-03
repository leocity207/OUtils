O::For_Each_In_Tuple
====================

Overview
--------
.. doxygenfunction:: O::For_Each_In_Tuple


Basic Usage
-----------
Iterating over each element:

.. code-block:: cpp

    auto t = std::make_tuple(1, 2.5, std::string("abc"));

    O::For_Each_In_Tuple(t, [](auto& elem) {
        // elem refers to each tuple element in order
        std::cout << elem << "\n";
    });

Modifying elements:

.. code-block:: cpp

    auto t = std::make_tuple(1, 2, 3);

    O::For_Each_In_Tuple(t, [](auto& elem) {
        elem *= 2;   // modifies tuple values in-place
    });

    // t becomes (2, 4, 6)
