///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2019, LAAS-CNRS
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef BINDINGS_PYTHON_CROCODDYL_MULTIBODY_ACTIONS_FREE_FWDDYN_HPP_
#define BINDINGS_PYTHON_CROCODDYL_MULTIBODY_ACTIONS_FREE_FWDDYN_HPP_

#include "crocoddyl/multibody/actions/free-fwddyn.hpp"

namespace crocoddyl {
namespace python {

namespace bp = boost::python;

void exposeDifferentialActionFreeFwdDynamics() {
  bp::class_<DifferentialActionModelFreeFwdDynamics, bp::bases<DifferentialActionModelAbstract> >(
      "DifferentialActionModelFreeFwdDynamics",
      "Differential action model for free forward dynamics in multibody systems.\n\n"
      "This class implements a the dynamics using Articulate Body Algorithm (ABA),\n"
      "or a custom implementation in case of system with armatures. If you want to\n"
      "include the armature, you need to use setArmature(). On the other hand, the\n"
      "stack of cost functions are implemented in CostModelSum().",
      bp::init<StateMultibody&, CostModelSum&>(bp::args(" self", " state", " costs"),
                                               "Initialize the free forward-dynamics action model.\n\n"
                                               ":param state: multibody state\n"
                                               ":param costs: stack of cost functions")
          [bp::with_custodian_and_ward<1, 2, bp::with_custodian_and_ward<1, 3> >()])
      .def("calc", &DifferentialActionModelFreeFwdDynamics::calc_wrap,
           DiffActionModel_calc_wraps(
               bp::args(" self", " data", " x", " u=None"),
               "Compute the next state and cost value.\n\n"
               "It describes the time-continuous evolution of the multibody system without any contact.\n"
               "Additionally it computes the cost value associated to this state and control pair.\n"
               ":param data: free forward-dynamics action data\n"
               ":param x: time-continuous state vector\n"
               ":param u: time-continuous control input"))
      .def<void (DifferentialActionModelFreeFwdDynamics::*)(
          const boost::shared_ptr<DifferentialActionDataAbstract>&, const Eigen::VectorXd&, const Eigen::VectorXd&,
          const bool&)>("calcDiff", &DifferentialActionModelFreeFwdDynamics::calcDiff_wrap,
                        bp::args(" self", " data", " x", " u=None", " recalc=True"),
                        "Compute the derivatives of the differential multibody system (free of contact) and\n"
                        "its cost functions.\n\n"
                        "It computes the partial derivatives of the differential multibody system and the\n"
                        "cost function. If recalc == True, it first updates the state evolution\n"
                        "and cost value. This function builds a quadratic approximation of the\n"
                        "action model (i.e. dynamical system and cost function).\n"
                        ":param data: free forward-dynamics action data\n"
                        ":param x: time-continuous state vector\n"
                        ":param u: time-continuous control input\n"
                        ":param recalc: If true, it updates the state evolution and the cost value.")
      .def<void (DifferentialActionModelFreeFwdDynamics::*)(const boost::shared_ptr<DifferentialActionDataAbstract>&,
                                                            const Eigen::VectorXd&, const Eigen::VectorXd&)>(
          "calcDiff", &DifferentialActionModelFreeFwdDynamics::calcDiff_wrap, bp::args(" self", " data", " x", " u"))
      .def<void (DifferentialActionModelFreeFwdDynamics::*)(const boost::shared_ptr<DifferentialActionDataAbstract>&,
                                                            const Eigen::VectorXd&)>(
          "calcDiff", &DifferentialActionModelFreeFwdDynamics::calcDiff_wrap, bp::args(" self", " data", " x"))
      .def<void (DifferentialActionModelFreeFwdDynamics::*)(const boost::shared_ptr<DifferentialActionDataAbstract>&,
                                                            const Eigen::VectorXd&, const bool&)>(
          "calcDiff", &DifferentialActionModelFreeFwdDynamics::calcDiff_wrap,
          bp::args(" self", " data", " x", " recalc"))
      .def("createData", &DifferentialActionModelFreeFwdDynamics::createData, bp::args(" self"),
           "Create the free forward dynamics differential action data.")
      .add_property(
          "pinocchio",
          bp::make_function(&DifferentialActionModelFreeFwdDynamics::get_pinocchio, bp::return_internal_reference<>()),
          "multibody model (i.e. pinocchio model)")
      .add_property(
          "costs",
          bp::make_function(&DifferentialActionModelFreeFwdDynamics::get_costs, bp::return_internal_reference<>()),
          "total cost model")
      .add_property("armature",
                    bp::make_function(&DifferentialActionModelFreeFwdDynamics::get_armature,
                                      bp::return_value_policy<bp::return_by_value>()),
                    bp::make_function(&DifferentialActionModelFreeFwdDynamics::set_armature),
                    "set an armature mechanism in the joints");

  bp::register_ptr_to_python<boost::shared_ptr<DifferentialActionDataFreeFwdDynamics> >();

  bp::class_<DifferentialActionDataFreeFwdDynamics, bp::bases<DifferentialActionDataAbstract> >(
      "DifferentialActionDataFreeFwdDynamics", "Action data for the differential LQR system.",
      bp::init<DifferentialActionModelFreeFwdDynamics*>(bp::args(" self", " model"),
                                                        "Create free forward-dynamics action data.\n\n"
                                                        ":param model: free forward-dynamics action model"))
      .add_property(
          "pinocchio",
          bp::make_getter(&DifferentialActionDataFreeFwdDynamics::pinocchio, bp::return_internal_reference<>()),
          "pinocchio data")
      .add_property("costs",
                    bp::make_getter(&DifferentialActionDataFreeFwdDynamics::costs,
                                    bp::return_value_policy<bp::return_by_value>()),
                    "total cost data");
}

}  // namespace python
}  // namespace crocoddyl

#endif  // BINDINGS_PYTHON_CROCODDYL_MULTIBODY_ACTIONS_FREE_FWDDYN_HPP_
