#include "che_obj.h"

#include <fstream>
#include <vector>
#include <cstring>
#include <cassert>


che_obj::che_obj(const string & file)
{
	init(file);
}

che_obj::che_obj(const che_obj & mesh): che(mesh)
{
}

void che_obj::read_file(const string & file)
{
	string soff;
	size_t n_v, n_f, v;

	ifstream is(file);

	assert(is.good());
	
	real_t x, y, z;
	index_t i, j, k;

	vector<vertex> vertices;
	vector<index_t> faces;

	char line[128];
	while(is.getline(line, sizeof(line)))
	{
		stringstream ss(line + 2);

		if(line[0] == 'v')
		{
			ss >> x >> y >> z;
			
			vertices.push_back({x, y, z});
		}

		if(line[0] == 'f')
		{
			ss >> i >> j >> k;
			
			faces.push_back(i - 1);	
			faces.push_back(j - 1);	
			faces.push_back(k - 1);	
		}
	}

	is.close();
	
	init(vertices.data(), vertices.size(), faces.data(), faces.size() / che::P);
}

void che_obj::write_file(const che * mesh, const string & file)
{
	ofstream os(file + ".obj");

	os << "####\n#\n";
	os << "# OBJ generated by gproshan 2019" << endl;
	os << "# vertices: " << mesh->n_vertices() << endl;
	os << "# faces: " << mesh->n_faces() << endl;
	os << "#\n####\n";

	for(size_t v = 0; v < mesh->n_vertices(); v++)
		os << "v " << mesh->gt(v) << endl;

	for(index_t he = 0; he < mesh->n_half_edges(); )
	{
		os << "f";
		for(index_t i = 0; i < che::P; i++)
			os << " " << mesh->vt(he++) + 1;
		os << endl;
	}

	os.close();
}

