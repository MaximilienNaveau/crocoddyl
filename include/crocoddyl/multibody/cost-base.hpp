///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2019, LAAS-CNRS
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef CROCODDYL_MULTIBODY_COST_BASE_HPP_
#define CROCODDYL_MULTIBODY_COST_BASE_HPP_

#include "crocoddyl/multibody/states/multibody.hpp"
#include "crocoddyl/core/activation-base.hpp"
#include <pinocchio/multibody/data.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace crocoddyl {

struct CostDataAbstract;  // forward declaration

class CostModelAbstract {
 public:
  CostModelAbstract(StateMultibody& state, ActivationModelAbstract& activation, unsigned int const& nu,
                    const bool& with_residuals = true);
  CostModelAbstract(StateMultibody& state, ActivationModelAbstract& activation, const bool& with_residuals = true);
  CostModelAbstract(StateMultibody& state, unsigned int const& nr, unsigned int const& nu,
                    const bool& with_residuals = true);
  CostModelAbstract(StateMultibody& state, unsigned int const& nr, const bool& with_residuals = true);
  ~CostModelAbstract();

  virtual void calc(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::Ref<const Eigen::VectorXd>& x,
                    const Eigen::Ref<const Eigen::VectorXd>& u) = 0;
  virtual void calcDiff(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::Ref<const Eigen::VectorXd>& x,
                        const Eigen::Ref<const Eigen::VectorXd>& u, const bool& recalc = true) = 0;
  virtual boost::shared_ptr<CostDataAbstract> createData(pinocchio::Data* const data);

  void calc(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::Ref<const Eigen::VectorXd>& x);
  void calcDiff(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::Ref<const Eigen::VectorXd>& x);

  StateMultibody& get_state() const;
  ActivationModelAbstract& get_activation() const;
  unsigned int const& get_nu() const;

 protected:
  StateMultibody& state_;
  ActivationModelAbstract& activation_;
  unsigned int nu_;
  bool with_residuals_;
  Eigen::VectorXd unone_;

#ifdef PYTHON_BINDINGS

 public:
  void calc_wrap(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::VectorXd& x,
                 const Eigen::VectorXd& u = Eigen::VectorXd()) {
    if (u.size() == 0) {
      calc(data, x);
    } else {
      calc(data, x, u);
    }
  }

  void calcDiff_wrap(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::VectorXd& x,
                     const Eigen::VectorXd& u, const bool& recalc) {
    calcDiff(data, x, u, recalc);
  }
  void calcDiff_wrap(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::VectorXd& x,
                     const Eigen::VectorXd& u) {
    calcDiff(data, x, u, true);
  }
  void calcDiff_wrap(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::VectorXd& x) {
    calcDiff(data, x, unone_, true);
  }
  void calcDiff_wrap(const boost::shared_ptr<CostDataAbstract>& data, const Eigen::VectorXd& x, const bool& recalc) {
    calcDiff(data, x, unone_, recalc);
  }

#endif
};

struct CostDataAbstract {
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  template <typename Model>
  CostDataAbstract(Model* const model, pinocchio::Data* const data)
      : pinocchio(data), activation(model->get_activation().createData()), cost(0.) {
    const int& ndx = model->get_state().get_ndx();
    const int& nu = model->get_nu();
    const int& nr = model->get_activation().get_nr();
    Lx = Eigen::VectorXd::Zero(ndx);
    Lu = Eigen::VectorXd::Zero(nu);
    Lxx = Eigen::MatrixXd::Zero(ndx, ndx);
    Lxu = Eigen::MatrixXd::Zero(ndx, nu);
    Luu = Eigen::MatrixXd::Zero(nu, nu);
    r = Eigen::VectorXd::Zero(nr);
    Rx = Eigen::MatrixXd::Zero(nr, ndx);
    Ru = Eigen::MatrixXd::Zero(nr, nu);
  }

  pinocchio::Data* pinocchio;
  boost::shared_ptr<ActivationDataAbstract> activation;
  double cost;
  Eigen::VectorXd Lx;
  Eigen::VectorXd Lu;
  Eigen::MatrixXd Lxx;
  Eigen::MatrixXd Lxu;
  Eigen::MatrixXd Luu;
  Eigen::VectorXd r;
  Eigen::MatrixXd Rx;
  Eigen::MatrixXd Ru;
};

}  // namespace crocoddyl

#endif  // CROCODDYL_MULTIBODY_COST_BASE_HPP_
