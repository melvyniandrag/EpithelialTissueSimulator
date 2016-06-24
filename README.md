NagaiHonda
==========

NagaiHondaModel

The ic.txt file holds the initial mesh.

# verts [v], # cells [c]
vert 0 (x_0, y_0, z_0)
vert 1 (x_1, y_1, z_1)
.
.
.
vert v-1 (x_{v-1}, y_{v-1}, z_{v-1})
cell 0 (# verts [m_0], vert # 0, vert # 1, . . . vert # m_0 -1)
.
.
.
cell c - 1 (# verts [m_c], vert # 0, vert # 1, . . . vert # m_c -1)