///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2019, LAAS-CNRS
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef BINDINGS_PYTHON_CROCODDYL_MULTIBODY_CONTACT_BASE_HPP_
#define BINDINGS_PYTHON_CROCODDYL_MULTIBODY_CONTACT_BASE_HPP_

#include "crocoddyl/multibody/contact-base.hpp"

namespace crocoddyl {
namespace python {

namespace bp = boost::python;

class ContactModelAbstract_wrap : public ContactModelAbstract, public bp::wrapper<ContactModelAbstract> {
 public:
  ContactModelAbstract_wrap(StateMultibody& state, int nc, int nu) : ContactModelAbstract(state, nc, nu) {}
  ContactModelAbstract_wrap(StateMultibody& state, int nc) : ContactModelAbstract(state, nc) {}

  void calc(const boost::shared_ptr<ContactDataAbstract>& data, const Eigen::Ref<const Eigen::VectorXd>& x) {
    assert(x.size() == state_.get_nx() && "x has wrong dimension");
    return bp::call<void>(this->get_override("calc").ptr(), data, (Eigen::VectorXd)x);
  }

  void calcDiff(const boost::shared_ptr<ContactDataAbstract>& data, const Eigen::Ref<const Eigen::VectorXd>& x,
                const bool& recalc = true) {
    assert(x.size() == state_.get_nx() && "x has wrong dimension");
    return bp::call<void>(this->get_override("calcDiff").ptr(), data, (Eigen::VectorXd)x, recalc);
  }

  void updateLagrangian(const boost::shared_ptr<ContactDataAbstract>& data, const Eigen::VectorXd& lambda) {
    assert(lambda.size() == nc_ && "lambda has wrong dimension");
    return bp::call<void>(this->get_override("updateLagrangian").ptr(), data, lambda);
  }
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ContactModel_calcDiff_wraps, ContactModelAbstract::calcDiff_wrap, 2, 3)

void exposeContactAbstract() {
  bp::class_<ContactModelAbstract_wrap, boost::noncopyable>(
      "ContactModelAbstract",
      "Abstract rigid contact model.\n\n"
      "It defines a template for rigid contact models based on acceleration-based holonomic constraints.\n"
      "The calc and calcDiff functions compute the contact Jacobian and drift (holonomic constraint) or\n"
      "the derivatives of the holonomic constraint, respectively.",
      bp::init<StateMultibody&, int, bp::optional<int> >(
          bp::args(" self", " state", " nc", " nu=state.nv"),
          "Initialize the contact model.\n\n"
          ":param state: state of the multibody system\n"
          ":param nc: dimension of contact model\n"
          ":param nu: dimension of the control vector")[bp::with_custodian_and_ward<1, 2>()])
      .def("calc", pure_virtual(&ContactModelAbstract_wrap::calc), bp::args(" self", " data", " x"),
           "Compute the contact Jacobian and drift.\n\n"
           "The rigid contact model throught acceleration-base holonomic constraint\n"
           "of the contact frame placement.\n"
           ":param data: contact data\n"
           ":param x: state vector")
      .def("calcDiff", pure_virtual(&ContactModelAbstract_wrap::calcDiff),
           bp::args(" self", " data", " x", " recalc=True"),
           "Compute the derivatives of contact holonomic constraint.\n\n"
           "The rigid contact model throught acceleration-base holonomic constraint\n"
           "of the contact frame placement.\n"
           ":param data: cost data\n"
           ":param x: state vector\n"
           ":param recalc: If true, it updates the contact Jacobian and drift.")
      .def("updateLagrangian", pure_virtual(&ContactModelAbstract_wrap::updateLagrangian),
           bp::args(" self", " data", " lambda"),
           "Convert the Lagrangian into a stack of spatial forces.\n\n"
           ":param data: cost data\n"
           ":param lambda: Lagrangian vector")
      .def("updateLagrangianDiff", &ContactModelAbstract_wrap::updateLagrangianDiff,
           bp::args(" self", " data", " Gx", " Gu"),
           "Update the Jacobian of the Lagrangian.\n\n"
           ":param data: cost data\n"
           ":param Gx: Jacobian of Lagrangian w.r.t. the state\n"
           ":param Gu: Jacobian of the Lagrangian w.r.t. the control")
      .def("createData", &ContactModelAbstract_wrap::createData, bp::with_custodian_and_ward_postcall<0, 2>(),
           bp::args(" self", " data"),
           "Create the contact data.\n\n"
           "Each contact model has its own data that needs to be allocated. This function\n"
           "returns the allocated data for a predefined cost.\n"
           ":param data: Pinocchio data\n"
           ":return contact data.")
      .add_property("state",
                    bp::make_function(&ContactModelAbstract_wrap::get_state, bp::return_internal_reference<>()),
                    "state of the multibody system")
      .add_property(
          "nc", bp::make_function(&ContactModelAbstract_wrap::get_nc, bp::return_value_policy<bp::return_by_value>()),
          "dimension of contact")
      .add_property(
          "nu", bp::make_function(&ContactModelAbstract_wrap::get_nu, bp::return_value_policy<bp::return_by_value>()),
          "dimension of control");

  bp::class_<ContactDataAbstract, boost::shared_ptr<ContactDataAbstract>, boost::noncopyable>(
      "ContactDataAbstract", "Abstract class for contact datas.\n\n",
      bp::init<ContactModelAbstract*, pinocchio::Data*>(
          bp::args(" self", " model", " data"),
          "Create common data shared between contact models.\n\n"
          ":param model: cost model\n"
          ":param data: Pinocchio data")[bp::with_custodian_and_ward<1, 3>()])
      .add_property("pinocchio", bp::make_getter(&ContactDataAbstract::pinocchio, bp::return_internal_reference<>()),
                    "pinocchio data")
      .add_property("Jc", bp::make_getter(&ContactDataAbstract::Jc, bp::return_value_policy<bp::return_by_value>()),
                    bp::make_setter(&ContactDataAbstract::Jc), "contact Jacobian")
      .add_property("a0", bp::make_getter(&ContactDataAbstract::a0, bp::return_value_policy<bp::return_by_value>()),
                    bp::make_setter(&ContactDataAbstract::a0), "contact drift")
      .add_property("Ax", bp::make_getter(&ContactDataAbstract::Ax, bp::return_value_policy<bp::return_by_value>()),
                    bp::make_setter(&ContactDataAbstract::Ax), "Jacobian of the contact constraint")
      .add_property("Gx", bp::make_getter(&ContactDataAbstract::Ax, bp::return_value_policy<bp::return_by_value>()),
                    bp::make_setter(&ContactDataAbstract::Ax), "Jacobian of the contact forces")
      .add_property("Gu", bp::make_getter(&ContactDataAbstract::Ax, bp::return_value_policy<bp::return_by_value>()),
                    bp::make_setter(&ContactDataAbstract::Ax), "Jacobian of the contact forces")
      .def_readwrite("joint", &ContactDataAbstract::joint, "joint index of the contact frame")
      .def_readwrite("f", &ContactDataAbstract::f, "external spatial forces");
}

}  // namespace python
}  // namespace crocoddyl

#endif  // BINDINGS_PYTHON_CROCODDYL_MULTIBODY_CONTACT_BASE_HPP_
