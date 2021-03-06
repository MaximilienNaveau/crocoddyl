///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2019, LAAS-CNRS
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "crocoddyl/core/solver-base.hpp"

namespace crocoddyl {

SolverAbstract::SolverAbstract(ShootingProblem& problem)
    : problem_(problem),
      is_feasible_(false),
      cost_(0.),
      stop_(0.),
      xreg_(NAN),
      ureg_(NAN),
      steplength_(1.),
      dV_(0.),
      dVexp_(0.),
      th_acceptstep_(0.1),
      th_stop_(1e-9),
      iter_(0) {
  // Allocate common data
  const unsigned int& T = problem_.get_T();
  xs_.resize(T + 1);
  us_.resize(T);
  models_.resize(T + 1);
  datas_.resize(T + 1);
  for (unsigned int t = 0; t < T; ++t) {
    ActionModelAbstract* model = problem_.running_models_[t];
    boost::shared_ptr<ActionDataAbstract>& data = problem_.running_datas_[t];
    const int& nu = model->get_nu();

    xs_[t] = model->get_state().zero();
    us_[t] = Eigen::VectorXd::Zero(nu);
    models_[t] = model;
    datas_[t] = data;
  }
  xs_.back() = problem_.terminal_model_->get_state().zero();
  models_.back() = problem_.terminal_model_;
  datas_.back() = problem_.terminal_data_;
}

SolverAbstract::~SolverAbstract() {}

void SolverAbstract::setCandidate(const std::vector<Eigen::VectorXd>& xs_warm,
                                  const std::vector<Eigen::VectorXd>& us_warm, const bool& is_feasible) {
  const unsigned int& T = problem_.get_T();

  if (xs_warm.size() == 0) {
    for (unsigned int t = 0; t < T; ++t) {
      xs_[t] = problem_.running_models_[t]->get_state().zero();
    }
    xs_.back() = problem_.terminal_model_->get_state().zero();
  } else {
    assert(xs_warm.size() == T + 1);
    std::copy(xs_warm.begin(), xs_warm.end(), xs_.begin());
  }

  if (us_warm.size() == 0) {
    for (unsigned int t = 0; t < T; ++t) {
      const int& nu = problem_.running_models_[t]->get_nu();
      us_[t] = Eigen::VectorXd::Zero(nu);
    }
  } else {
    assert(us_warm.size() == T);
    std::copy(us_warm.begin(), us_warm.end(), us_.begin());
  }
  is_feasible_ = is_feasible;
}

void SolverAbstract::setCallbacks(const std::vector<CallbackAbstract*>& callbacks) { callbacks_ = callbacks; }

const ShootingProblem& SolverAbstract::get_problem() const { return problem_; }

const std::vector<ActionModelAbstract*>& SolverAbstract::get_models() const { return models_; }

const std::vector<boost::shared_ptr<ActionDataAbstract> >& SolverAbstract::get_datas() const { return datas_; }

const std::vector<Eigen::VectorXd>& SolverAbstract::get_xs() const { return xs_; }

const std::vector<Eigen::VectorXd>& SolverAbstract::get_us() const { return us_; }

const bool& SolverAbstract::get_isFeasible() const { return is_feasible_; }

const unsigned int& SolverAbstract::get_iter() const { return iter_; }

const double& SolverAbstract::get_cost() const { return cost_; }

const double& SolverAbstract::get_stop() const { return stop_; }

const Eigen::Vector2d& SolverAbstract::get_d() const { return d_; }

const double& SolverAbstract::get_xreg() const { return xreg_; }

const double& SolverAbstract::get_ureg() const { return ureg_; }

const double& SolverAbstract::get_stepLength() const { return steplength_; }

const double& SolverAbstract::get_dV() const { return dV_; }

const double& SolverAbstract::get_dVexp() const { return dVexp_; }

bool raiseIfNaN(const double& value) {
  if (std::isnan(value) || std::isinf(value) || value >= 1e30) {
    return true;
  } else {
    return false;
  }
}

}  // namespace crocoddyl
