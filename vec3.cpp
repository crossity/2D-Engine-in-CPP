struct vec3 {
    float x, y, z;

    vec3() {
        this->x = 0.f;
        this->y = 0.f;
        this->z = 0.f;
    }
    vec3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};