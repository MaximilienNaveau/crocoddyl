///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2019, LAAS-CNRS
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef CROCODDYL_CORE_SOLVERS_DDP_HPP_
#define CROCODDYL_CORE_SOLVERS_DDP_HPP_

#include <Eigen/Cholesky>
#include <vector>
#include "crocoddyl/core/solver-base.hpp"

namespace crocoddyl {

class SolverDDP : public SolverAbstract {
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  explicit SolverDDP(ShootingProblem& problem);
  ~SolverDDP();

  bool solve(const std::vector<Eigen::VectorXd>& init_xs = DEFAULT_VECTOR,
             const std::vector<Eigen::VectorXd>& init_us = DEFAULT_VECTOR, unsigned int const& maxiter = 100,
             const bool& is_feasible = false, const double& regInit = 1e-9);
  void computeDirection(const bool& recalc = true);
  double tryStep(const double& steplength = 1);
  double stoppingCriteria();
  const Eigen::Vector2d& expectedImprovement();
  double calc();
  void backwardPass();
  void forwardPass(const double& stepLength);

  const std::vector<Eigen::MatrixXd>& get_Vxx() const;
  const std::vector<Eigen::VectorXd>& get_Vx() const;
  const std::vector<Eigen::MatrixXd>& get_Qxx() const;
  const std::vector<Eigen::MatrixXd>& get_Qxu() const;
  const std::vector<Eigen::MatrixXd>& get_Quu() const;
  const std::vector<Eigen::VectorXd>& get_Qx() const;
  const std::vector<Eigen::VectorXd>& get_Qu() const;
  const std::vector<Eigen::MatrixXd>& get_K() const;
  const std::vector<Eigen::VectorXd>& get_k() const;
  const std::vector<Eigen::VectorXd>& get_gaps() const;

 private:
  void computeGains(unsigned int const& t);
  void increaseRegularization();
  void decreaseRegularization();
  void allocateData();

 protected:
  double regfactor_;
  double regmin_;
  double regmax_;

  double cost_try_;
  std::vector<Eigen::VectorXd> xs_try_;
  std::vector<Eigen::VectorXd> us_try_;
  std::vector<Eigen::VectorXd> dx_;

  // allocate data
  std::vector<Eigen::MatrixXd> Vxx_;
  std::vector<Eigen::VectorXd> Vx_;
  std::vector<Eigen::MatrixXd> Qxx_;
  std::vector<Eigen::MatrixXd> Qxu_;
  std::vector<Eigen::MatrixXd> Quu_;
  std::vector<Eigen::VectorXd> Qx_;
  std::vector<Eigen::VectorXd> Qu_;
  std::vector<Eigen::MatrixXd> K_;
  std::vector<Eigen::VectorXd> k_;
  std::vector<Eigen::VectorXd> gaps_;

 private:
  Eigen::VectorXd xnext_;
  Eigen::VectorXd x_reg_;
  Eigen::MatrixXd FxTVxx_p_;
  std::vector<Eigen::MatrixXd> FuTVxx_p_;
  Eigen::VectorXd fTVxx_p_;
  std::vector<Eigen::LLT<Eigen::MatrixXd> > Quu_llt_;
  std::vector<Eigen::VectorXd> Quuk_;
  std::vector<double> alphas_;
  double th_grad_;
  double th_step_;
  bool was_feasible_;
};

}  // namespace crocoddyl

#endif  // CROCODDYL_CORE_SOLVERS_DDP_HPP_
