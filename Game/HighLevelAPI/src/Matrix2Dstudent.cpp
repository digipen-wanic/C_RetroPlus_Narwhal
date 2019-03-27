//==================================================================-
/*
/file   Matrix2DStudent.cpp
/author Jakob McFarland
/date   11/28/2018
/brief

This is the implementation file for all member functions
of the class Matrix2D, as specified in specification
file Matrix2DStudent.h.

*/
//==================================================================-

#include "stdafx.h"
#include "Matrix2DStudent.h"

namespace CS230
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor for matrix. Sets all data in matrix to 0.
	Matrix2D::Matrix2D()
	{
		for (unsigned row = 0; row < 3; ++row)
		{
			for (unsigned col = 0; col < 3; ++col)
			{
				m[row][col] = 0.0f;
			}
		}
	}
	// This function creates an identity matrix and returns it.
	Matrix2D Matrix2D::IdentityMatrix()
	{
		Matrix2D matrix;

		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;

		return matrix;
	}

	// This function calculates the transpose matrix of Mtx and saves it in the result matrix.
	// (NOTE: Care must be taken when pResult = pMtx.)
	Matrix2D Matrix2D::Transposed() const
	{
		Matrix2D matrix;

		for (unsigned row = 0; row < 3; ++row)
		{
			for (unsigned col = 0; col < 3; ++col)
			{
				matrix.m[row][col] = m[col][row];
			}
		}

		return matrix;
	}

	// This function creates a translation matrix from x & y and returns it.
	Matrix2D Matrix2D::TranslationMatrix(float x, float y)
	{
		Matrix2D matrix = Matrix2D::IdentityMatrix();

		matrix.m[0][2] = x;
		matrix.m[1][2] = y;

		return matrix;
	}

	// This function creates a scaling matrix from x & y and returns it.
	Matrix2D Matrix2D::ScalingMatrix(float x, float y)
	{
		Matrix2D matrix = Matrix2D::IdentityMatrix();

		matrix.m[0][0] = x;
		matrix.m[1][1] = y;

		return matrix;
	}

	// This matrix creates a rotation matrix from "Angle" whose value is in degrees.
	// Return the resulting matrix.
	// Converting from degrees to radians can be performed as follows:
	//	 radians = (angle * M_PI) / 180.0f
	Matrix2D Matrix2D::RotationMatrixDegrees(float angle)
	{
		float radians = static_cast<float>(angle * M_PI) / 180.0f;

		Matrix2D matrix = Matrix2D::IdentityMatrix();

		matrix.m[0][0] = cosf(radians);
		matrix.m[1][1] = cosf(radians);

		matrix.m[0][1] = sinf(radians);
		matrix.m[1][0] = -sinf(radians);

		return matrix;
	}

	// This matrix creates a rotation matrix from "Angle" whose value is in radians.
	// Return the resulting matrix.
	Matrix2D Matrix2D::RotationMatrixRadians(float angle)
	{
		Matrix2D matrix = Matrix2D::IdentityMatrix();

		matrix.m[0][0] = cosf(angle);
		matrix.m[1][1] = cosf(angle);

		matrix.m[0][1] = -sinf(angle);
		matrix.m[1][0] = sinf(angle);

		return matrix;
	}

	// This function multiplies the current matrix with another and returns the result as a new matrix.
	// HINT: Use the RowColumnMultiply function to help write this function.
	Matrix2D Matrix2D::operator*(const Matrix2D& other) const
	{
		Matrix2D matrix;

		for (unsigned row = 0; row < 3; ++row)
		{
			for (unsigned col = 0; col < 3; ++col)
			{
				matrix.m[row][col] = RowColumnMultiply(other, row, col);
			}
		}

		return matrix;
	}

	// This function multiplies the current matrix with another, storing the result in the current matrix.
	// HINT: Use the RowColumnMultiply function to help write this function.
	Matrix2D& Matrix2D::operator*=(const Matrix2D& other)
	{
		for (unsigned row = 0; row < 3; ++row)
		{
			for (unsigned col = 0; col < 3; ++col)
			{
				m[row][col] = RowColumnMultiply(other, row, col);
			}
		}

		return *this;
	}

	// This function multiplies a matrix with a vector and returns the result as a new vector.
	// Result = Mtx * Vec.
	Vector2D Matrix2D::operator*(const Vector2D& vec) const
	{
		Vector2D vector;

		vector.x = m[0][0] * vec.x + m[1][0] * vec.y + m[0][2];
		vector.y = m[0][1] * vec.x + m[1][1] * vec.y + m[1][2];

		return vector;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Multiplies a row of the current matrix by a column in another and returns the result.
	float Matrix2D::RowColumnMultiply(const Matrix2D& other, unsigned row, unsigned col) const
	{
		float total = 0;
		for (int i = 0; i < 3; ++i)
		{
			total += m[row][i] * other.m[i][col];
		}

		return total;
	}
}