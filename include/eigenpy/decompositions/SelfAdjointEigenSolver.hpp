/*
 * Copyright 2020 INRIA
 */

#ifndef __eigenpy_decomposition_self_adjoint_eigen_solver_hpp__
#define __eigenpy_decomposition_self_adjoint_eigen_solver_hpp__

#include "eigenpy/eigenpy.hpp"

#include <Eigen/Core>
#include <Eigen/Eigenvalues>

#include "eigenpy/utils/scalar-name.hpp"

namespace eigenpy
{
  
  template<typename _MatrixType>
  struct SelfAdjointEigenSolverVisitor
  : public boost::python::def_visitor< SelfAdjointEigenSolverVisitor<_MatrixType> >
  {
    
    typedef _MatrixType MatrixType;
    typedef typename MatrixType::Scalar Scalar;
    typedef Eigen::SelfAdjointEigenSolver<MatrixType> Solver;
    
    template<class PyClass>
    void visit(PyClass& cl) const
    {
      namespace bp = boost::python;
      cl
      .def(bp::init<>("Default constructor"))
      .def(bp::init<Eigen::DenseIndex>(bp::arg("size"),
                                       "Default constructor with memory preallocation"))
      .def(bp::init<MatrixType,bp::optional<int> >(bp::args("matrix","options"),
                                                   "Computes eigendecomposition of given matrix"))
       
      .def("eigenvalues",&Solver::eigenvalues,bp::arg("self"),
           "Returns the eigenvalues of given matrix.",
           bp::return_value_policy<bp::return_by_value>())
      .def("eigenvectors",&Solver::eigenvectors,bp::arg("self"),
           "Returns the eigenvectors of given matrix.",
           bp::return_value_policy<bp::return_by_value>())
      
      .def("compute",&SelfAdjointEigenSolverVisitor::compute_proxy<MatrixType>,
           bp::args("self","matrix"),
           "Computes the eigendecomposition of given matrix.",
           bp::return_value_policy<bp::reference_existing_object>())
      .def("compute",(Solver & (Solver::*)(const Eigen::EigenBase<MatrixType> & matrix, int options))&Solver::compute,
           bp::args("self","matrix","options"),
           "Computes the eigendecomposition of given matrix.",
           bp::return_value_policy<bp::reference_existing_object>())
      
      .def("computeDirect",&SelfAdjointEigenSolverVisitor::computeDirect_proxy,
           bp::args("self","matrix"),
           "Computes eigendecomposition of given matrix using a closed-form algorithm.",
           bp::return_value_policy<bp::reference_existing_object>())
      .def("computeDirect",(Solver & (Solver::*)(const MatrixType & matrix, int options))&Solver::computeDirect,
           bp::args("self","matrix","options"),
           "Computes eigendecomposition of given matrix using a closed-form algorithm.",
           bp::return_value_policy<bp::reference_existing_object>())
       
      .def("operatorInverseSqrt",&Solver::operatorInverseSqrt,bp::arg("self"),
           "Computes the inverse square root of the matrix.",
           bp::return_value_policy<bp::return_by_value>())
      .def("operatorSqrt",&Solver::operatorSqrt,bp::arg("self"),
           "Computes the inverse square root of the matrix.",
           bp::return_value_policy<bp::return_by_value>())
      
      .def("info",&Solver::info,bp::arg("self"),
           "NumericalIssue if the input contains INF or NaN values or overflow occured. Returns Success otherwise.")
      ;
    }
    
    static void expose()
    {
      static const std::string classname = "SelfAdjointEigenSolver" + scalar_name<Scalar>::shortname();
      expose(classname);
    }
    
    static void expose(const std::string & name)
    {
      namespace bp = boost::python;
      bp::class_<Solver>(name.c_str(),
                         bp::no_init)
      .def(SelfAdjointEigenSolverVisitor());
    }
    
  private:
    
    template<typename MatrixType>
    static Solver & compute_proxy(Solver & self, const Eigen::EigenBase<MatrixType> & matrix)
    {
      return self.compute(matrix);
    }
    
    static Solver & computeDirect_proxy(Solver & self, const MatrixType & matrix)
    {
      return self.computeDirect(matrix);
    }
    
  };
  
} // namespace eigenpy

#endif // ifndef __eigenpy_decomposition_self_adjoint_eigen_solver_hpp__


