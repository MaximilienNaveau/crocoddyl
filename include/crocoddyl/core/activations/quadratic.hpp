///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2019, LAAS-CNRS
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef CROCODDYL_CORE_ACTIVATIONS_QUADRATIC_HPP_
#define CROCODDYL_CORE_ACTIVATIONS_QUADRATIC_HPP_

#include "crocoddyl/core/activation-base.hpp"

namespace crocoddyl {

class ActivationModelQuad : public ActivationModelAbstract {
 public:
  explicit ActivationModelQuad(unsigned int const& nr);
  ~ActivationModelQuad();

  void calc(const boost::shared_ptr<ActivationDataAbstract>& data, const Eigen::Ref<const Eigen::VectorXd>& r);
  void calcDiff(const boost::shared_ptr<ActivationDataAbstract>& data, const Eigen::Ref<const Eigen::VectorXd>& r,
                const bool& recalc = true);
  boost::shared_ptr<ActivationDataAbstract> createData();
};

}  // namespace crocoddyl

#endif  // CROCODDYL_CORE_ACTIVATIONS_QUADRATIC_HPP_
