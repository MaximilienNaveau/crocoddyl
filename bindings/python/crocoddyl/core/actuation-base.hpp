///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2019, LAAS-CNRS
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef BINDINGS_PYTHON_CROCODDYL_CORE_ACTUATION_BASE_HPP_
#define BINDINGS_PYTHON_CROCODDYL_CORE_ACTUATION_BASE_HPP_

#include "crocoddyl/core/actuation-base.hpp"

namespace crocoddyl {
namespace python {

namespace bp = boost::python;

class ActuationModelAbstract_wrap : public ActuationModelAbstract, public bp::wrapper<ActuationModelAbstract> {
 public:
  ActuationModelAbstract_wrap(StateAbstract& state, unsigned int const& nu)
      : ActuationModelAbstract(state, nu), bp::wrapper<ActuationModelAbstract>() {}

  void calc(const boost::shared_ptr<ActuationDataAbstract>& data, const Eigen::Ref<const Eigen::VectorXd>& x,
            const Eigen::Ref<const Eigen::VectorXd>& u) {
    assert(x.size() == state_.get_nx() && "x has wrong dimension");
    assert(u.size() == nu_ && "u has wrong dimension");
    return bp::call<void>(this->get_override("calc").ptr(), data, (Eigen::VectorXd)x, (Eigen::VectorXd)u);
  }

  void calcDiff(const boost::shared_ptr<ActuationDataAbstract>& data, const Eigen::Ref<const Eigen::VectorXd>& x,
                const Eigen::Ref<const Eigen::VectorXd>& u, const bool& recalc = true) {
    assert(x.size() == state_.get_nx() && "x has wrong dimension");
    assert(u.size() == nu_ && "u has wrong dimension");
    return bp::call<void>(this->get_override("calcDiff").ptr(), data, (Eigen::VectorXd)x, (Eigen::VectorXd)u, recalc);
  }
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ActuationModel_calcDiff_wraps, ActuationModelAbstract::calcDiff_wrap, 3, 4)

void exposeActuationAbstract() {
  bp::class_<ActuationModelAbstract_wrap, boost::noncopyable>(
      "ActuationModelAbstract",
      "Abstract class for actuation models.\n\n"
      "In crocoddyl, an actuation model is a block that takes u and outputs a (in continouos\n"
      "time), where a is the actuation signal of our system, and it also computes the derivatives\n"
      "of this model. These computations are mainly carry on inside calc() and calcDiff(),\n"
      "respectively.",
      bp::init<StateAbstract&, int>(bp::args(" self", " state", " nu"),
                                    "Initialize the actuation model.\n\n"
                                    ":param state: state description,\n"
                                    ":param nu: dimension of control vector")[bp::with_custodian_and_ward<1, 2>()])
      .def("calc", pure_virtual(&ActuationModelAbstract_wrap::calc), bp::args(" self", " data", " x", " u"),
           "Compute the actuation signal from the control input u.\n\n"
           "It describes the time-continuos evolution of the actuation model.\n"
           ":param data: actuation data\n"
           ":param x: state vector\n"
           ":param u: control input")
      .def("calcDiff", pure_virtual(&ActuationModelAbstract_wrap::calcDiff),
           bp::args(" self", " data", " x", " u", " recalc=True"),
           "Compute the derivatives of the actuation model.\n\n"
           "It computes the partial derivatives of the actuation model which is\n"
           "describes in continouos time.\n"
           ":param data: actuation data\n"
           ":param x: state vector\n"
           ":param u: control input\n"
           ":param recalc: If true, it updates the actuation signal.")
      .def("createData", &ActuationModelAbstract_wrap::createData, bp::args(" self"),
           "Create the actuation data.\n\n"
           "Each actuation model (AM) has its own data that needs to be allocated.\n"
           "This function returns the allocated data for a predefined AM.\n"
           ":return AM data.")
      .add_property(
          "nu",
          bp::make_function(&ActuationModelAbstract_wrap::get_nu, bp::return_value_policy<bp::return_by_value>()),
          "dimension of control vector")
      .add_property("state",
                    bp::make_function(&ActuationModelAbstract_wrap::get_state, bp::return_internal_reference<>()),
                    "state");

  bp::register_ptr_to_python<boost::shared_ptr<ActuationDataAbstract> >();

  bp::class_<ActuationDataAbstract, boost::noncopyable>(
      "ActuationDataAbstract",
      "Abstract class for actuation datas.\n\n"
      "In crocoddyl, an actuation data contains all the required information for processing an\n"
      "user-defined actuation model. The actuation data typically is allocated onces by running\n"
      "model.createData().",
      bp::init<ActuationModelAbstract*>(bp::args(" self", " model"),
                                        "Create common data shared between actuation models.\n\n"
                                        "The actuation data uses the model in order to first process it.\n"
                                        ":param model: actuation model"))
      .add_property("a", bp::make_getter(&ActuationDataAbstract::a, bp::return_value_policy<bp::return_by_value>()),
                    bp::make_setter(&ActuationDataAbstract::a), "actuation signal")
      .add_property("Ax", bp::make_getter(&ActuationDataAbstract::Ax, bp::return_value_policy<bp::return_by_value>()),
                    bp::make_setter(&ActuationDataAbstract::Ax), "Jacobian of the actuation model")
      .add_property("Au", bp::make_getter(&ActuationDataAbstract::Au, bp::return_value_policy<bp::return_by_value>()),
                    bp::make_setter(&ActuationDataAbstract::Au), "Jacobian of the actuation model");
}

}  // namespace python
}  // namespace crocoddyl

#endif  // BINDINGS_PYTHON_CROCODDYL_CORE_ACTUATION_BASE_HPP_
