///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (C) 2018-2020, LAAS-CNRS, University of Edinburgh
// Copyright note valid unless otherwise stated in individual files.
// All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "crocoddyl/core/utils/exception.hpp"
#include "crocoddyl/multibody/impulses/impulse-3d.hpp"

#include <pinocchio/algorithm/frames.hpp>
#include <pinocchio/algorithm/kinematics-derivatives.hpp>

namespace crocoddyl {

template <typename Scalar>
ImpulseModel3DTpl<Scalar>::ImpulseModel3DTpl(boost::shared_ptr<StateMultibody> state, const std::size_t& frame)
    : Base(state, 3), frame_(frame) {}

template <typename Scalar>
ImpulseModel3DTpl<Scalar>::~ImpulseModel3DTpl() {}

template <typename Scalar>
void ImpulseModel3DTpl<Scalar>::calc(const boost::shared_ptr<ImpulseDataAbstract>& data,
                                     const Eigen::Ref<const VectorXs>&) {
  boost::shared_ptr<ImpulseData3D> d = boost::static_pointer_cast<ImpulseData3D>(data);

  pinocchio::getFrameJacobian(state_->get_pinocchio(), *d->pinocchio, frame_, pinocchio::LOCAL, d->fJf);
  d->Jc = d->fJf.template topRows<3>();
}

template <typename Scalar>
void ImpulseModel3DTpl<Scalar>::calcDiff(const boost::shared_ptr<ImpulseDataAbstract>& data,
                                         const Eigen::Ref<const VectorXs>&) {
  boost::shared_ptr<ImpulseData3D> d = boost::static_pointer_cast<ImpulseData3D>(data);
  pinocchio::getJointVelocityDerivatives(state_->get_pinocchio(), *d->pinocchio, d->joint, pinocchio::LOCAL,
                                         d->v_partial_dq, d->v_partial_dv);
  d->dv0_dq.noalias() = d->fXj.template topRows<3>() * d->v_partial_dq;
}

template <typename Scalar>
void ImpulseModel3DTpl<Scalar>::updateForce(const boost::shared_ptr<ImpulseDataAbstract>& data,
                                            const VectorXs& force) {
  if (force.size() != 3) {
    throw_pretty("Invalid argument: "
                 << "lambda has wrong dimension (it should be 3)");
  }
  boost::shared_ptr<ImpulseData3D> d = boost::static_pointer_cast<ImpulseData3D>(data);
  data->f = d->jMf.act(pinocchio::ForceTpl<Scalar>(force, Vector3s::Zero()));
}

template <typename Scalar>
boost::shared_ptr<ImpulseDataAbstractTpl<Scalar> > ImpulseModel3DTpl<Scalar>::createData(
    pinocchio::DataTpl<Scalar>* const data) {
  return boost::make_shared<ImpulseData3D>(this, data);
}

template <typename Scalar>
const std::size_t& ImpulseModel3DTpl<Scalar>::get_frame() const {
  return frame_;
}

}  // namespace crocoddyl