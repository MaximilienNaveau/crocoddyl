///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2019, LAAS-CNRS
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "crocoddyl/core/optctrl/shooting.hpp"

namespace crocoddyl {

ShootingProblem::ShootingProblem(const Eigen::VectorXd& x0, const std::vector<ActionModelAbstract*>& running_models,
                                 ActionModelAbstract* const terminal_model)
    : terminal_model_(terminal_model),
      running_models_(running_models),
      T_(static_cast<unsigned int>(running_models.size())),
      x0_(x0),
      cost_(0.) {
  assert(x0_.size() == running_models_[0]->get_state().get_nx() && "x0 has wrong dimension");
  allocateData();
}

ShootingProblem::~ShootingProblem() {}

double ShootingProblem::calc(const std::vector<Eigen::VectorXd>& xs, const std::vector<Eigen::VectorXd>& us) {
  assert(xs.size() == T_ + 1 && "Wrong dimension of the state trajectory, it should be T + 1.");
  assert(us.size() == T_ && "Wrong dimension of the control trajectory, it should be T.");

  cost_ = 0;
  for (unsigned int i = 0; i < T_; ++i) {
    ActionModelAbstract* model = running_models_[i];
    boost::shared_ptr<ActionDataAbstract>& data = running_datas_[i];
    const Eigen::VectorXd& x = xs[i];
    const Eigen::VectorXd& u = us[i];

    model->calc(data, x, u);
    cost_ += data->cost;
  }
  terminal_model_->calc(terminal_data_, xs.back());
  cost_ += terminal_data_->cost;
  return cost_;
}

double ShootingProblem::calcDiff(const std::vector<Eigen::VectorXd>& xs, const std::vector<Eigen::VectorXd>& us) {
  assert(xs.size() == T_ + 1 && "Wrong dimension of the state trajectory, it should be T + 1.");
  assert(us.size() == T_ && "Wrong dimension of the control trajectory, it should be T.");

  cost_ = 0;
  for (unsigned int i = 0; i < T_; ++i) {
    ActionModelAbstract* model = running_models_[i];
    boost::shared_ptr<ActionDataAbstract>& data = running_datas_[i];
    const Eigen::VectorXd& x = xs[i];
    const Eigen::VectorXd& u = us[i];

    model->calcDiff(data, x, u);
    cost_ += data->cost;
  }
  terminal_model_->calcDiff(terminal_data_, xs.back());
  cost_ += terminal_data_->cost;
  return cost_;
}

void ShootingProblem::rollout(const std::vector<Eigen::VectorXd>& us, std::vector<Eigen::VectorXd>& xs) {
  assert(us.size() == T_ && "Wrong dimension of the control trajectory, it should be T.");

  xs.resize(T_ + 1);
  xs[0] = x0_;
  for (unsigned int i = 0; i < T_; ++i) {
    ActionModelAbstract* model = running_models_[i];
    boost::shared_ptr<ActionDataAbstract>& data = running_datas_[i];
    const Eigen::VectorXd& x = xs[i];
    const Eigen::VectorXd& u = us[i];

    model->calc(data, x, u);
    xs[i + 1] = data->get_xnext();
  }
  terminal_model_->calc(terminal_data_, xs.back());
}

std::vector<Eigen::VectorXd> ShootingProblem::rollout_us(const std::vector<Eigen::VectorXd>& us) {
  std::vector<Eigen::VectorXd> xs;
  rollout(us, xs);
  return xs;
}

unsigned int ShootingProblem::get_T() const { return T_; }

const Eigen::VectorXd& ShootingProblem::get_x0() const { return x0_; }

void ShootingProblem::allocateData() {
  for (unsigned int i = 0; i < T_; ++i) {
    ActionModelAbstract* model = running_models_[i];
    running_datas_.push_back(model->createData());
  }
  terminal_data_ = terminal_model_->createData();
}

std::vector<ActionModelAbstract*>& ShootingProblem::get_runningModels() { return running_models_; }

ActionModelAbstract* ShootingProblem::get_terminalModel() { return terminal_model_; }

std::vector<boost::shared_ptr<ActionDataAbstract> >& ShootingProblem::get_runningDatas() { return running_datas_; }

boost::shared_ptr<ActionDataAbstract>& ShootingProblem::get_terminalData() { return terminal_data_; }

}  // namespace crocoddyl
