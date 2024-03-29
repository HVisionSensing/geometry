/*
_______________________________________________________________________
__________________________ G E O M E T R Y ____________________________
|
| THIS FILE IS PART OF THE GEOMETRY TEMPLATE LIBRARY.
| USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     
| GOVERNED BY A BSD-STYLE SOURCE LICENSE.
| PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       
_______________________________________________________________________
_______________________________________________________________________
*/

#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include <gtl/gtl.hpp>
#include <gtl/vec3.hpp>
#include <gtl/ray.hpp>
#include <gtl/matrix3.hpp>

namespace gtl
{
#define MIN_VERTICES 4	// minimum number of vertices required to have a polyhedron

    /*!
    \class Polyhedron Polyhedron.hpp gtl/Polyhedron.hpp
    \brief Represents a Polyhedron as a collection of 3D points
    \ingroup base
    */
    template<typename Type>
    class Polyhedron
    {
    public:
        //! The default constructor makes a tetrahedron.
        Polyhedron()
        {
			Vec3<Type> pts[4];

			m_vertices = NULL;
			m_num_vertices = 0;

			pts[0].setValue(-1, -1, 0);
			pts[1].setValue(1, -1, 0);
			pts[2].setValue(0, 1, 0);
			pts[3].setValue(0, 0, 1);

			setVertices(pts, 4);
        }

        //!	Constructs a polyhedron from the provided list of 3D points.
        Polyhedron(const Vec3<Type> *pts, int num_points)
        {
			m_vertices = NULL;
			m_num_vertices = 0;

            if (setVertices(pts, num_points) < 0)
				Polyhedron();	// if setVertices fails, construct default polyhedron...
        }

        //! Default destructor frees the internal list of 3D points.
        virtual ~Polyhedron()
		{
			if (m_vertices != NULL)
				delete [] m_vertices;
		}

		//! Returns the number of vertices declared for this polyhedron.
		int getNumVertices()
		{
			return m_num_vertices;
		}

        //! Reset the vertices describing the polyhedron (must provide more than 3 vertices)
        int setVertices(const Vec3<Type> *pts, int num_points)
        {
			if (num_points < MIN_VERTICES)
				return -1;

			if (m_vertices != NULL)
				delete [] m_vertices;

			m_vertices = new Vec3<Type> [num_points];
			m_num_vertices = num_points;

			for (int i = 0; i < m_num_vertices; i++)
			{
				m_vertices[i].setValue(pts[i].x(), pts[i].y(), pts[i].z());
			}

			return 0;
		}

        //! Get a reference to the specified vertex
        void getVertex(int vertex_index, Vec3<Type> &vertex)
        {
			if (vertex_index < 0 || vertex_index >= m_num_vertices)
			{
				return;
			}

			vertex.setValue(m_vertices[vertex_index].x(), 
							m_vertices[vertex_index].y(),
							m_vertices[vertex_index].z());
		}

		//! Perform a rotation of the polyhedron around an arbitrary axis by the specified angle in radians
		void rotate(Vec3<Type> axis, Vec3<Type> point, double angle)
		{
			Type q[4];

			// Calculate the rotation quaternion
			q[0] = cos(angle / 2.0);
			q[1] = sin(angle / 2.0) * axis.x();
			q[2] = sin(angle / 2.0) * axis.y();
			q[3] = sin(angle / 2.0) * axis.z();

			// And now the rotation matrix
			Matrix3<Type> matrix(SQR(q[0]) + SQR(q[1]) - SQR(q[2]) - SQR(q[3]),		2 * (q[1] * q[2] - q[0] * q[3]),				2 * (q[1] * q[3] + q[0] * q[2]),
								2 * (q[2] * q[1] + q[0] * q[3]),					SQR(q[0]) - SQR(q[1]) + SQR(q[2]) - SQR(q[3]),	2 * (q[2] * q[3] - q[0] * q[1]),
								2 * (q[3] * q[1] - q[0] * q[2]),					2 * (q[3] * q[2] + q[0] * q[1]),				SQR(q[0]) - SQR(q[1]) - SQR(q[2]) + SQR(q[3]));

			// Perform the actual rotation on all vertices
			for (int v = 0; v < m_num_vertices; v++)
			{
				Vec3<Type> rotated;

				m_vertices[v] -= point;	// translate to origin
				matrix.multMatrixVec(m_vertices[v], rotated);	// rotate around origin
				m_vertices[v] = rotated + point;	// translate back to initial point
			}
		}

    protected:
        Vec3<Type> *m_vertices;
		int			m_num_vertices;
    };

    typedef Polyhedron<int>    Polyhedroni;
    typedef Polyhedron<float>  Polyhedronf;
    typedef Polyhedron<double> Polyhedrond;
} // namespace gtl

#endif
