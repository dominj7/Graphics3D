# Total 104/147

# House  2/2

16.10.2023  OK 

# Colors 5/5

16.10.2023  OK

# Indices 5/5

23.10.2023 11:30:50 OK
Ale proszę usunąć wykomentowane wierzchołki.
I proszę zmienić nazwe projektu na Indices (duża pierwsza litera).

# Uniforms 8/8

30.10.2023 12:00:43 OK

# PVM 5/5

6.11.2023 11:38:27 OK

# Pyramid 4/4

6.11.2023 11:39:37 OK

# Resize 8/8

2023-11-13 13:05:44 OK

# Zoom 10/10

20.11.2023 18:14:34 OK

# Camera movement 15/15

27.11.2023 11:38:14
W metodzie `frame` najpier podpinają Państo buffor `u_trans_buffer_handle` do punkti 1, a potem do punktu 0. To ostatnie nie ma sensu. Chyba mieli Państwo na myśli `OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, 0))`, ale wtedy to musi być wywołane dopiero po `drawElements`. 

4.12.2023 14:14:14 OK

# Mesh 10/10

4.12.2023 14:15:04 OK

# Kd material 15/20

15.01.2024 18:01:36 OK

# Textures 12/15

15.01.2024 18:04:01 OK

# OBJ reader 5/5

15.01.2024 18:04:56 OK



