#include "daal.h"
#include <iostream>

using namespace daal;
using namespace daal::algorithms;
using namespace daal::data_management;
using namespace daal::services;

const size_t dimension = 3;
double inputArray[dimension *dimension] =
  {
    1.0,  2.0,  4.0,
    2.0, 13.0, 23.0,
    4.0, 23.0, 77.0
  };

int main(int argc, char *argv[])
{
  /* Create input numeric table from array */
  SharedPtr<NumericTable> inputData = SharedPtr<NumericTable>(new Matrix<double>(dimension, dimension, inputArray));

  /*  Create the algorithm object for computation of the Cholesky decomposition using the default method */
  cholesky::Batch<> algorithm;

  /* Set input for the algorithm */
  algorithm.input.set(cholesky::data, inputData);

  /* Compute Cholesky decomposition */
  algorithm.compute();

  /* Get pointer to Cholesky factor */
  SharedPtr<Matrix<double> > factor =
    staticPointerCast<Matrix<double>, NumericTable>(algorithm.getResult()->get(cholesky::choleskyFactor));

/* Print the first element of the Cholesky factor */
std::cout << "The first element of the Cholesky factor: " << (*factor)[0][0];

return 0;
}
