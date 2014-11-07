#include "register.hpp"

#pragma warning (push)
#pragma warning (disable:4482)	/* nonstandard extension used: enum 'enum' used in qualified name */



_VGR_BEGIN
_VGR_ALGO_BEGIN



///	<description>
///	Estimates rotation matrix using least-square fitting of two 3d point sets
///	stored in matrices. The quantity of points of each point set is identical.
///	</description>
///	<parameters>
///		<param name='src'> Matrix storing oordinates of source points. </param>
///		<param name='dst'> Matrix storing oordinates of destination points. </param>
///		<param name='rotation' direction='out'> Rotation matrix. </param>
///	</parameters>
///	<returns>
///	True if the rotation matrix is reliable. Computation can be unstable if the
///	points in either point set are nearly colinear.
///	</returns>
bool
Register::rotationMatrix(
		const eigen::Matrix3xN &src,
		const eigen::Matrix3xN &dst,
		eigen::Matrix3 *rotation) {

	eigen::JacobiSVD<eigen::Matrix3> solver(3, 3, eigen::DecompositionOptions::ComputeFullU | eigen::DecompositionOptions::ComputeFullV);
	solver.compute(src * dst.transpose());
	bool ret_code = true;
	if (Util::zero(solver.singularValues()[0])
			|| Util::zero((solver.singularValues()[1] + solver.singularValues()[2]) / solver.singularValues()[0])) {
		ret_code = false;
	}
	rotation->noalias() = solver.matrixV() * solver.matrixU().transpose();
	if (rotation->determinant() < 0) {
		eigen::Matrix3 matrix_v(solver.matrixV());
		matrix_v.col(2) = -matrix_v.col(2);
		rotation->noalias() = matrix_v * solver.matrixU().transpose();
	}
	return (ret_code);
}

///	<description>
///	Estimates rotation matrix using principal component analysis (PCA) on two 3d
///	point sets stored in matrices. The quantity of points of one point sets can
///	be different with the other one.
///	</description>
///	<parameters>
///		<param name='src'> Matrix storing oordinates of source points. </param>
///		<param name='dst'> Matrix storing oordinates of destination points. </param>
///		<param name='rotation' direction='out'> Rotation matrix. </param>
///	</parameters>
void
Register::rotationMatrix2(
		const eigen::Matrix3xN &src,
		const eigen::Matrix3xN &dst,
		eigen::Matrix3 *rotation) {
	
	eigen::JacobiSVD<eigen::Matrix3> src_solver(3, 3, eigen::DecompositionOptions::ComputeFullU | eigen::DecompositionOptions::ComputeFullV);
	eigen::JacobiSVD<eigen::Matrix3> dst_solver(3, 3, eigen::DecompositionOptions::ComputeFullU | eigen::DecompositionOptions::ComputeFullV);
	src_solver.compute(src * src.transpose());
	dst_solver.compute(dst * dst.transpose());
	rotation->noalias() = dst_solver.matrixV() * src_solver.matrixU().transpose();
	if (rotation->determinant() < 0) {
		eigen::Matrix3 matrix_v(dst_solver.matrixV());
		matrix_v.col(1) = -matrix_v.col(1);
		rotation->noalias() = matrix_v * src_solver.matrixU().transpose();
	}
}

///	<description>
///	Estimates rotation matrix and translation vector using least-square fitting
///	of two 3d point sets stored in matrices. The quantity of points of each point
///	set is identical.
///	</description>
///	<parameters>
///		<param name='src'> Matrix storing oordinates of source points. </param>
///		<param name='dst'> Matrix storing oordinates of destination points. </param>
///		<param name='rotation' direction='out'> Rotation matrix. </param>
///		<param name='translation' direction='out'> Translation vector. </param>
///	</parameters>
///	<returns>
///	True if the rotation matrix is reliable. Computation can be unstable if the
///	points in either point set are nearly colinear.
///	</returns>
bool
Register::transformation(
		const eigen::Matrix3xN &src,
		const eigen::Matrix3xN &dst,
		eigen::Matrix3 *rotation,
		eigen::Vector3 *translation) {

	eigen::Vector3 src_center = src.rowwise().mean();
	eigen::Vector3 dst_center = dst.rowwise().mean();
	eigen::Matrix3xN src_centered = src.colwise() - src_center;
	eigen::Matrix3xN dst_centered = dst.colwise() - dst_center;
	rotation->setZero();
	bool ret_code = Register::rotationMatrix(src_centered, dst_centered, rotation);
	translation->noalias() = dst_center - (*rotation) * src_center;
	return (ret_code);
}

///	<description>
///	Estimates rotation matrix and translation vector using principal component
///	analysis (PCA) on two 3d point sets stored in matrices. The quantity of
///	points of one point sets can be different with the other one.
///	</description>
///	<parameters>
///		<param name='src'> Matrix storing oordinates of source points. </param>
///		<param name='dst'> Matrix storing oordinates of destination points. </param>
///		<param name='rotation' direction='out'> Rotation matrix. </param>
///		<param name='translation' direction='out'> Translation vector. </param>
///	</parameters>
void
Register::transformation2(
		const eigen::Matrix3xN &src,
		const eigen::Matrix3xN &dst,
		eigen::Matrix3 *rotation,
		eigen::Vector3 *translation) {

	eigen::Vector3 src_center = src.rowwise().mean();
	eigen::Vector3 dst_center = dst.rowwise().mean();
	eigen::Matrix3xN src_centered = src.colwise() - src_center;
	eigen::Matrix3xN dst_centered = dst.colwise() - dst_center;
	rotation->setZero();
	Register::rotationMatrix2(src_centered, dst_centered, rotation);
	eigen::Matrix3xN src_rotated = (*rotation) * src;
	translation->noalias() = ((dst.rowwise().maxCoeff() + dst.rowwise().minCoeff())
			- (src_rotated.rowwise().maxCoeff() + src_rotated.rowwise().minCoeff())) * 0.5;
}



_VGR_ALGO_END
_VGR_END



#pragma warning (pop)
