///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2019, LAAS-CNRS
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef BINDINGS_PYTHON_CROCODDYL_CORE_SOLVERS_DDP_HPP_
#define BINDINGS_PYTHON_CROCODDYL_CORE_SOLVERS_DDP_HPP_

#include "crocoddyl/core/solvers/ddp.hpp"

namespace crocoddyl {
namespace python {

namespace bp = boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SolverDDP_solves, SolverDDP::solve, 0, 5)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SolverDDP_computeDirections, SolverDDP::computeDirection, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SolverDDP_trySteps, SolverDDP::tryStep, 0, 1)

void exposeSolverDDP() {
  bp::class_<SolverDDP, bp::bases<SolverAbstract> >(
      "SolverDDP",
      "DDP solver.\n\n"
      "The DDP solver computes an optimal trajectory and control commands by iterates\n"
      "running backward and forward passes. The backward-pass updates locally the\n"
      "quadratic approximation of the problem and computes descent direction,\n"
      "and the forward-pass rollouts this new policy by integrating the system dynamics\n"
      "along a tuple of optimized control commands U*.\n"
      ":param shootingProblem: shooting problem (list of action models along trajectory.)",
      bp::init<ShootingProblem&>(bp::args(" self", " problem"),
                                 "Initialize the vector dimension.\n\n"
                                 ":param problem: shooting problem.")[bp::with_custodian_and_ward<1, 2>()])
      .def("solve", &SolverDDP::solve,
           SolverDDP_solves(
               bp::args(" self", " init_xs=[]", " init_us=[]", " maxiter=100", " isFeasible=False", " regInit=None"),
               "Compute the optimal trajectory xopt, uopt as lists of T+1 and T terms.\n\n"
               "From an initial guess init_xs,init_us (feasible or not), iterate\n"
               "over computeDirection and tryStep until stoppingCriteria is below\n"
               "threshold. It also describes the globalization strategy used\n"
               "during the numerical optimization.\n"
               ":param init_xs: initial guess for state trajectory with T+1 elements.\n"
               ":param init_us: initial guess for control trajectory with T elements.\n"
               ":param maxiter: maximun allowed number of iterations.\n"
               ":param isFeasible: true if the init_xs are obtained from integrating the init_us (rollout).\n"
               ":param regInit: initial guess for the regularization value. Very low values are typical\n"
               "                used with very good guess points (init_xs, init_us).\n"
               ":returns the optimal trajectory xopt, uopt and a boolean that describes if convergence was reached."))
      .def("computeDirection", &SolverDDP::computeDirection,
           SolverDDP_computeDirections(
               bp::args(" self", " recalc=True"),
               "Compute the search direction (dx, du) for the current guess (xs, us).\n\n"
               "You must call setCandidate first in order to define the current\n"
               "guess. A current guess defines a state and control trajectory\n"
               "(xs, us) of T+1 and T elements, respectively.\n"
               ":params recalc: true for recalculating the derivatives at current state and control.\n"
               ":returns the search direction dx, du and the dual lambdas as lists of T+1, T and T+1 lengths."))
      .def("tryStep", &SolverDDP::tryStep,
           SolverDDP_trySteps(bp::args(" self", " stepLength=1"),
                              "Rollout the system with a predefined step length.\n\n"
                              ":param stepLength: step length\n"
                              ":returns the cost improvement."))
      .def("stoppingCriteria", &SolverDDP::stoppingCriteria, bp::args(" self"),
           "Return a sum of positive parameters whose sum quantifies the DDP termination.")
      .def("expectedImprovement", &SolverDDP::expectedImprovement, bp::return_value_policy<bp::copy_const_reference>(),
           bp::args(" self"),
           "Return two scalars denoting the quadratic improvement model\n\n"
           "For computing the expected improvement, you need to compute first\n"
           "the search direction by running computeDirection. The quadratic\n"
           "improvement model is described as dV = f_0 - f_+ = d1*a + d2*a**2/2.")
      .def("calc", &SolverDDP::calc, bp::args(" self"),
           "Update the Jacobian and Hessian of the optimal control problem\n\n"
           "These derivatives are computed around the guess state and control\n"
           "trajectory. These trajectory can be set by using setCandidate.\n"
           ":return the total cost around the guess trajectory.")
      .def("backwardPass", &SolverDDP::backwardPass, bp::args(" self"),
           "Run the backward pass (Riccati sweep)\n\n"
           "It assumes that the Jacobian and Hessians of the optimal control problem have been\n"
           "compute. These terms are computed by running calc.")
      .def("forwardPass", &SolverDDP::forwardPass, bp::args(" self", " stepLength=1"),
           "Run the forward pass or rollout\n\n"
           "It rollouts the action model give the computed policy (feedfoward terns and feedback\n"
           "gains) by the backwardPass. We can define different step lengths\n"
           ":param stepLength: applied step length (<= 1. and >= 0.)")
      .add_property("Vxx", make_function(&SolverDDP::get_Vxx, bp::return_value_policy<bp::copy_const_reference>()),
                    "Vxx")
      .add_property("Vx", make_function(&SolverDDP::get_Vx, bp::return_value_policy<bp::copy_const_reference>()), "Vx")
      .add_property("Qxx", make_function(&SolverDDP::get_Qxx, bp::return_value_policy<bp::copy_const_reference>()),
                    "Qxx")
      .add_property("Qxu", make_function(&SolverDDP::get_Qxu, bp::return_value_policy<bp::copy_const_reference>()),
                    "Qxu")
      .add_property("Quu", make_function(&SolverDDP::get_Quu, bp::return_value_policy<bp::copy_const_reference>()),
                    "Quu")
      .add_property("Qx", make_function(&SolverDDP::get_Qx, bp::return_value_policy<bp::copy_const_reference>()), "Qx")
      .add_property("Qu", make_function(&SolverDDP::get_Qu, bp::return_value_policy<bp::copy_const_reference>()), "Qu")
      .add_property("K", make_function(&SolverDDP::get_K, bp::return_value_policy<bp::copy_const_reference>()), "K")
      .add_property("k", make_function(&SolverDDP::get_k, bp::return_value_policy<bp::copy_const_reference>()), "k");
}

}  // namespace python
}  // namespace crocoddyl

#endif  // BINDINGS_PYTHON_CROCODDYL_CORE_SOLVERS_DDP_HPP_
