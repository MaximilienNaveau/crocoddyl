///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2020-2021, LAAS-CNRS, University of Edinburgh, INRIA
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include <pinocchio/algorithm/rnea-derivatives.hpp>
#include <pinocchio/algorithm/rnea.hpp>

namespace crocoddyl {

template <typename Scalar>
CostModelControlGravContactTpl<Scalar>::CostModelControlGravContactTpl(
    boost::shared_ptr<StateMultibody> state, boost::shared_ptr<ActivationModelAbstract> activation,
    const std::size_t nu)
    : Base(boost::make_shared<StateMultibody>(*state), activation, boost::make_shared<ResidualModelContactControlGrav>(boost::make_shared<StateMultibody>(*state), nu)) {
  std::cerr << "Deprecated CostModelControlGravContact: Use ResidualModelContactControlGrav with "
               "CostModelResidual class"
            << std::endl;
  if (activation_->get_nr() != state_->get_nv()) {
    throw_pretty("Invalid argument: "
                 << "nr is equals to " + std::to_string(state_->get_nv()));
  }
}

template <typename Scalar>
CostModelControlGravContactTpl<Scalar>::CostModelControlGravContactTpl(
    boost::shared_ptr<StateMultibody> state, boost::shared_ptr<ActivationModelAbstract> activation)
    : Base(boost::make_shared<StateMultibody>(*state), activation, boost::make_shared<ResidualModelContactControlGrav>(boost::make_shared<StateMultibody>(*state))) {
  std::cerr << "Deprecated CostModelControlGravContact: Use ResidualModelContactControlGrav with "
               "CostModelResidual class"
            << std::endl;
  if (activation_->get_nr() != state_->get_nv()) {
    throw_pretty("Invalid argument: "
                 << "nr is equals to " + std::to_string(state_->get_nv()));
  }
}

template <typename Scalar>
CostModelControlGravContactTpl<Scalar>::CostModelControlGravContactTpl(boost::shared_ptr<StateMultibody> state,
                                                                       std::size_t nu)
    : Base(boost::make_shared<StateMultibody>(*state), boost::make_shared<ResidualModelContactControlGrav>(boost::make_shared<StateMultibody>(*state), nu)) {
  std::cerr << "Deprecated CostModelControlGravContact: Use ResidualModelContactControlGrav with "
               "CostModelResidual class"
            << std::endl;
}

template <typename Scalar>
CostModelControlGravContactTpl<Scalar>::CostModelControlGravContactTpl(boost::shared_ptr<StateMultibody> state)
    : Base(boost::make_shared<StateMultibody>(*state), boost::make_shared<ResidualModelContactControlGrav>(boost::make_shared<StateMultibody>(*state))) {
  std::cerr << "Deprecated CostModelControlGravContact: Use ResidualModelContactControlGrav with "
               "CostModelResidual class"
            << std::endl;
}

template <typename Scalar>
CostModelControlGravContactTpl<Scalar>::~CostModelControlGravContactTpl() {}

}  // namespace crocoddyl
