///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2019, LAAS-CNRS
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef BINDINGS_PYTHON_CROCODDYL_MULTIBODY_CONTACTS_MULTIPLE_CONTACTS_HPP_
#define BINDINGS_PYTHON_CROCODDYL_MULTIBODY_CONTACTS_MULTIPLE_CONTACTS_HPP_

#include <functional>
#include <map>
#include <memory>
#include <utility>
#include <string>
#include "crocoddyl/multibody/contacts/multiple-contacts.hpp"

namespace crocoddyl {
namespace python {

namespace bp = boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ContactModelMultiple_calcDiff_wraps, ContactModelMultiple::calcDiff_wrap, 2, 3)

void exposeContactMultiple() {
  // Register custom converters between std::map and Python dict
  typedef boost::shared_ptr<ContactDataAbstract> ContactDataPtr;
  bp::to_python_converter<std::map<std::string, ContactItem, std::less<std::string>,
                                   std::allocator<std::pair<const std::string, ContactItem> > >,
                          map_to_dict<std::string, ContactItem> >();
  bp::to_python_converter<std::map<std::string, ContactDataPtr, std::less<std::string>,
                                   std::allocator<std::pair<const std::string, ContactDataPtr> > >,
                          map_to_dict<std::string, ContactDataPtr, false> >();
  dict_to_map<std::string, ContactItem>().from_python();
  dict_to_map<std::string, ContactDataPtr>().from_python();

  bp::class_<ContactItem, boost::noncopyable>(
      "ContactItem", "Describe a contact item.\n\n",
      bp::init<std::string, ContactModelAbstract*>(
          bp::args(" self", " name", " contact"),
          "Initialize the contact item.\n\n"
          ":param name: contact name\n"
          ":param contact: contact model")[bp::with_custodian_and_ward<1, 3>()])
      .def_readwrite("name", &ContactItem::name, "contact name")
      .add_property("contact", bp::make_getter(&ContactItem::contact, bp::return_internal_reference<>()),
                    "contact model");

  bp::class_<ContactModelMultiple, boost::noncopyable>(
      "ContactModelMultiple", bp::init<StateMultibody&, bp::optional<int> >(
                                  bp::args(" self", " state", " nu=state.nv"),
                                  "Initialize the multiple contact model.\n\n"
                                  ":param state: state of the multibody system\n"
                                  ":param nu: dimension of control vector")[bp::with_custodian_and_ward<1, 2>()])
      .def("addContact", &ContactModelMultiple::addContact, bp::with_custodian_and_ward<1, 3>(),
           bp::args(" self", " name", " contact"),
           "Add a contact item.\n\n"
           ":param name: contact name\n"
           ":param contact: contact model")
      .def("removeContact", &ContactModelMultiple::removeContact, bp::args(" self", " name"),
           "Remove a contact item.\n\n"
           ":param name: contact name")
      .def("calc", &ContactModelMultiple::calc_wrap, bp::args(" self", " data", " x"),
           "Compute the total contact Jacobian and drift.\n\n"
           "The rigid contact model throught acceleration-base holonomic constraint\n"
           "of the contact frame placement.\n"
           ":param data: contact data\n"
           ":param x: state vector")
      .def("calcDiff", &ContactModelMultiple::calcDiff_wrap,
           ContactModelMultiple_calcDiff_wraps(
               bp::args(" self", " data", " x", " recalc=True"),
               "Compute the derivatives of the total contact holonomic constraint.\n\n"
               "The rigid contact model throught acceleration-base holonomic constraint\n"
               "of the contact frame placement.\n"
               ":param data: cost data\n"
               ":param x: state vector\n"
               ":param recalc: If true, it updates the contact Jacobian and drift."))
      .def("updateLagrangian", &ContactModelMultiple::updateLagrangian, bp::args(" self", " data", " lambda"),
           "Convert the Lagrangian into a stack of spatial forces.\n\n"
           ":param data: cost data\n"
           ":param lambda: Lagrangian vector")
      .def("updateLagrangianDiff", &ContactModelMultiple::updateLagrangianDiff,
           bp::args(" self", " data", " Gx", " Gu"),
           "Update the Jacobian of the Lagrangian.\n\n"
           ":param data: cost data\n"
           ":param Gx: Jacobian of Lagrangian w.r.t. the state\n"
           ":param Gu: Jacobian of the Lagrangian w.r.t. the control")
      .def("createData", &ContactModelMultiple::createData, bp::with_custodian_and_ward_postcall<0, 2>(),
           bp::args(" self", " data"),
           "Create the total contact data.\n\n"
           ":param data: Pinocchio data\n"
           ":return total contact data.")
      .add_property(
          "contacts",
          bp::make_function(&ContactModelMultiple::get_contacts, bp::return_value_policy<bp::return_by_value>()),
          "stack of contacts")
      .add_property("state", bp::make_function(&ContactModelMultiple::get_state, bp::return_internal_reference<>()),
                    "state of the multibody system")
      .add_property("nc",
                    bp::make_function(&ContactModelMultiple::get_nc, bp::return_value_policy<bp::return_by_value>()),
                    "dimension of the total contact vector")
      .add_property("nu",
                    bp::make_function(&ContactModelMultiple::get_nu, bp::return_value_policy<bp::return_by_value>()),
                    "dimension of control vector");

  bp::class_<ContactDataMultiple, boost::shared_ptr<ContactDataMultiple>, bp::bases<ContactDataAbstract> >(
      "ContactDataMultiple", "Data class for multiple contacts.\n\n",
      bp::init<ContactModelMultiple*, pinocchio::Data*>(
          bp::args(" self", " model", " data"),
          "Create multicontact data.\n\n"
          ":param model: multicontact model\n"
          ":param data: Pinocchio data")[bp::with_custodian_and_ward<1, 2, bp::with_custodian_and_ward<1, 3> >()])
      .add_property("contacts",
                    bp::make_getter(&ContactDataMultiple::contacts, bp::return_value_policy<bp::return_by_value>()),
                    "stack of contacts data")
      .def_readwrite("fext", &ContactDataMultiple::fext, "external spatial forces");
}

}  // namespace python
}  // namespace crocoddyl

#endif  // BINDINGS_PYTHON_CROCODDYL_MULTIBODY_CONTACTS_MULTIPLE_CONTACTS_HPP_
