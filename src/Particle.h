#ifndef PARTICLE_H
#define PARTICLE_H

struct Particle{
    glm::vec3 pos, speed; //pozycja predkosc
    unsigned int r,g,b,a; // Color
    float size, angle, weight; //rozmiar itd
    float life; //czas zycia czasteczki
    float cameradistance; // Dystans od kamery jesli jest zbyt daleko to czasteczka umiera

    bool operator<(const Particle& that) const {
        // Sortowanie w odwrotnej kolejnosci by byly rysowane najpierw czasteczki ktore sa daleko od kamery
        return this->cameradistance > that.cameradistance;
    }
};

class Particles{
protected:
    GLuint particle;
    double currentTime;
    double delta;
    double lastTime;
    int LastUsedParticle = 0;
    int newparticles;
    float spread = 1.5f;
    int particleIndex;
    int ParticlesCount;
    GLubyte* g_particule_color_data;
    GLfloat* g_particule_position_size_data;

    glm::vec3 maindir;
    glm::vec3 randomdir;

    //Maksymalna ilosc czasteczek
    static const int MaxParticles = 10000;
    Particle ParticlesContainer[MaxParticles];
public:
    // wierzcholki VBO
    constexpr static const GLfloat g_vertex_buffer_data[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
    };

    // Funkcaj do znajdowania czasteczek które zgineły
    int FindUnusedParticle(){

        for(int i=LastUsedParticle; i<MaxParticles; i++){
            if (ParticlesContainer[i].life < 0){
                LastUsedParticle = i;
                return i;
            }
        }

        for(int i=0; i<LastUsedParticle; i++){
            if (ParticlesContainer[i].life < 0){
                LastUsedParticle = i;
                return i;
            }
        }

        return 0; // Napisz pierwsza czasteczke ktore znajdziesz
    }

    //Funkcja do sortowania czasteczek
    void SortParticles(){
        std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
    }
    //Funkcja do ustawiania kolorow czasteczek
    void particleSetColor(unsigned int red, unsigned int green,unsigned int blue, unsigned int alpha)
    {
        ParticlesContainer[particleIndex].r = red;
        ParticlesContainer[particleIndex].g = green;
        ParticlesContainer[particleIndex].b = blue;
        ParticlesContainer[particleIndex].a = alpha;
    }
    //Funkcja do ustawienia rozmiaru czasteczek
    void particleSetSize(float size){
        ParticlesContainer[particleIndex].size = size;
    }
    //Funkcja do ustawiania czasu zycia czasteczki
    void particleSetLife(float life){
        ParticlesContainer[particleIndex].life = life;
    }
    //Funkcja do ustawiania pozycji emitera czasteczek
    void particleSetPos(float x,float y,float z) {
        ParticlesContainer[particleIndex].pos = glm::vec3(x,y,z);
    }
    //Funkcja do ustawiania kierunku z predkoscia wyrzucania czasteczek z emitera
    void particleSetSpeed(float x,float y,float z){
        ParticlesContainer[particleIndex].speed = glm::vec3(x,y,z) *spread;
    }
};

#endif //PARTICLE_H
