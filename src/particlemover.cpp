#include "particlemover.h"
#include "system.h"
#include "particles.h"
#include "cpelapsedtimer.h"
#include "grid.h"
#include "settings.h"
#include "random.h"
#include <cassert>
#include <cmath>
using namespace std;

ParticleMover::ParticleMover() :
    m_system(0),
    m_grid(0),
    m_particles(0),
    m_random(0),
    m_sqrtWallTemperatureOverMass(0),
    m_isInitialized(false)
{

}

void ParticleMover::initialize(System *system)
{
    m_system = system;
    m_grid = system->grid();
    m_particles = system->particles();
    m_random = system->random();
    m_sqrtWallTemperatureOverMass = sqrt(system->settings()->temperature / system->settings()->massPerParticle());

    m_isInitialized = true;
}

void ParticleMover::doMove(unsigned int particleIndex, double dt)
{
    m_particles->x[particleIndex] += m_particles->vx[particleIndex]*dt;
    m_particles->y[particleIndex] += m_particles->vy[particleIndex]*dt;
}

#define printCondition (m_system->numberOfTimesteps() == 0 && particleIndex==6013)

//void ParticleMover::moveParticle(unsigned int particleIndex, double dt, unsigned int depth)
//{
//#ifdef DSMC_DEBUG
//    cout.precision(15);
//    if(printCondition) cout << endl << endl << "Working with new particle " << particleIndex << " with depth=" << depth << " at r=[" << m_particles->x[particleIndex] << ", " << m_particles->y[particleIndex] << "] and dt=" << dt << endl;
//    if(printCondition) cout << "v=[" << m_particles->vx[particleIndex] << ", " << m_particles->vy[particleIndex] << "]" << endl;
//#endif
//    double tau = dt;
//    unsigned int voxelIndex = m_grid->indexPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]);
//    unsigned char voxelType = m_grid->voxelPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]);
//    if(voxelType != +VoxelType::Empty) {
//        cout << "Warning, an invalid situation occured with particle " << particleIndex << " in timestep " << m_system->numberOfTimesteps() << " and depth=" << depth << ", maybe too large timestep?" << endl;
//        exit(1);
//    }

//#ifdef DSMC_DEBUG
//    if(printCondition) cout << "This particle is on a voxel with index " << voxelIndex << " and type " << int(voxelType) << endl;
//#endif

//    doMove(particleIndex, tau);
//    voxelType = m_grid->voxelPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]);
//    voxelIndex = m_grid->indexPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]);
//#ifdef DSMC_DEBUG
//    if(printCondition) cout << "Moved particle to a voxel with index " << voxelIndex << " and type " << int(voxelType) << endl;
//    if(printCondition) cout << "Now at r=[" << m_particles->x[particleIndex] << ", " << m_particles->y[particleIndex] << "]" << endl;
//#endif
//    int count = 0;

//    if(voxelType >= +VoxelType::Wall) {
//        while(voxelType != +VoxelType::Surface) {
//            if(++count > 100) {
//                cout << "Warning, we found an infinite loop with molecule " << particleIndex << " at timestep " << m_system->numberOfTimesteps() << " on a wall" << endl;
//                exit(1);
//            }
//#ifdef DSMC_DEBUG
//            if(printCondition) cout << "Inside while != surface" << endl;
//#endif
//            if(voxelType == +VoxelType::Wall) {
//                tau /= 2;
//                doMove(particleIndex, -tau);
//                voxelType = m_grid->voxelPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]);
//                voxelIndex = m_grid->indexPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]);
//#ifdef DSMC_DEBUG
//                if(printCondition) cout << "It is a wall, we moved back to a voxel with index " << voxelIndex << " and type " << int(voxelType) << endl;
//#endif
//            } else {
//                dt -= tau;
//#ifdef DSMC_DEBUG
//                if(printCondition) cout << "It is a surface, we will recursively call this function" << endl;
//#endif
//                if(dt > 1e-5 && depth < 10) {
//                    moveParticle(particleIndex, dt, depth+1);
//                }
//                return;
//            }
//        }

//        if(voxelType == +VoxelType::Empty) {
//            cout << "Error 1: We hit a wall, but managed to skip the surface voxel. Decrease your timestep." << endl;
//            exit(1);
//        }

//        unsigned int collisionVoxelIndex = m_grid->indexPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]);
//#ifdef DSMC_DEBUG
//        if(printCondition) cout << "We have now reached a voxel with index " << collisionVoxelIndex << " and type " << int(m_grid->voxelWithIndex(collisionVoxelIndex)) << endl;
//#endif
//        count = 0;
//        while(voxelType == +VoxelType::Surface) {
//            if(++count > 100) {
//                cout << "Warning, we found an infinite loop with molecule " << particleIndex << " at timestep " << m_system->numberOfTimesteps() << " on a surface" << endl;
//                exit(1);
//            }

//            collisionVoxelIndex = m_grid->indexPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]);
//            doMove(particleIndex, -tau); // Move back outside surface voxel
//#ifdef DSMC_DEBUG
//            if(printCondition) cout << "Moved back with tau=" << -tau << ", now at voxel with index " << m_grid->indexPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]) << " and type " << int(m_grid->voxelPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex])) << endl;
//#endif
//            tau = m_grid->timeUntilCollision(collisionVoxelIndex, vec2(m_particles->x[particleIndex], m_particles->y[particleIndex]), vec2(m_particles->vx[particleIndex], m_particles->vy[particleIndex])) - 1e-6;
//#ifdef DSMC_DEBUG
//            if(printCondition) cout << "Collision time is " << tau << endl;
//            cout.precision(15);

//            if(printCondition) cout << "1: Position before we move is r=[" << m_particles->x[particleIndex] << ", " << m_particles->y[particleIndex] << "]" << endl;
//#endif
//            doMove(particleIndex, tau); // Move close to surface
//#ifdef DSMC_DEBUG
//            if(printCondition) cout << "1: Position after we moved is r=[" << m_particles->x[particleIndex] << ", " << m_particles->y[particleIndex] << "]" << endl;
//            if(printCondition) cout << "1: Moved to voxel surface, we are now at voxel with index " << m_grid->indexPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]) << " and type " << int(m_grid->voxelPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex])) << endl;
//#endif
//            voxelType = m_grid->voxelPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]);
//        }

//        // We're not at the boundary anymore, so we can move over here and do happy colliding
//        dt -= tau;

//        if(m_grid->voxelWithIndex(collisionVoxelIndex) != +VoxelType::Surface) {
//            cout << "Error 2: We hit a wall, but managed to skip the surface voxel. Decrease your timestep." << endl;
//            exit(1);
//        }

//        // Calculate surface collision
//        float v_normal = m_sqrtWallTemperatureOverMass*sqrt(-2.0*log(m_random->nextFloat()));
//        float v_tangent = m_random->nextGaussian(0, m_sqrtWallTemperatureOverMass);
//#ifdef DSMC_DEBUG
//        if(printCondition)cout << "Normal: [" << m_grid->normal(collisionVoxelIndex, 0) << ", " << m_grid->normal(collisionVoxelIndex, 1) << "]" << endl;
//        if(printCondition)cout << "Tangent: [" << m_grid->tangent(collisionVoxelIndex, 0) << ", " << m_grid->tangent(collisionVoxelIndex, 1) << "]" << endl;
//        if(printCondition)cout << "Old velocity: [" << m_particles->vx[particleIndex] << ", " << m_particles->vy[particleIndex] << "]" << endl;
//#endif
//        m_particles->vx[particleIndex] = v_normal*m_grid->normal(collisionVoxelIndex, 0) + v_tangent*m_grid->tangent(collisionVoxelIndex, 0);
//        m_particles->vy[particleIndex] = v_normal*m_grid->normal(collisionVoxelIndex, 1) + v_tangent*m_grid->tangent(collisionVoxelIndex, 1);
//#ifdef DSMC_DEBUG
//        if(printCondition)cout << "New velocity: [" << m_particles->vx[particleIndex] << ", " << m_particles->vy[particleIndex] << "]" << endl;
//        if(printCondition)cout << "dt remaining: " << dt << endl;
//        if(printCondition)cout << "We are now at voxel with index " << m_grid->indexPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]) << " and type " << int(m_grid->voxelPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex])) << endl;
//        if(printCondition)cout << "Moved to a voxel with index " << m_grid->indexPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]) << " and type " << int(m_grid->voxelPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex])) << endl;
//#endif
//    } else dt=0;


//    if(dt > 1e-5 && depth < 10) {
//        moveParticle(particleIndex, dt, depth+1);
//    }
//}

void ParticleMover::moveParticle(unsigned int particleIndex, double dt, unsigned int depth)
{
#ifdef DSMC_DEBUG
    cout.precision(15);
    if(printCondition) cout << endl << endl << "Working with new particle " << particleIndex << " with depth=" << depth << " and dt=" << dt << endl;
    if(printCondition) cout << "r=[" << m_particles->x[particleIndex] << ", " << m_particles->y[particleIndex] << "]" << ";" << endl;
    if(printCondition) cout << "v=[" << m_particles->vx[particleIndex] << ", " << m_particles->vy[particleIndex] << "]" << ";" << endl;
#endif
    double tau = dt;
    unsigned int voxelIndex = m_grid->indexPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]);
    unsigned char voxelType = m_grid->voxelPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]);
    if(voxelType != +VoxelType::Empty) {
        cout << "Warning, an invalid situation occured with particle " << particleIndex << " in timestep " << m_system->numberOfTimesteps() << " and depth=" << depth << ", maybe too large timestep?" << endl;
        exit(1);
    }

#ifdef DSMC_DEBUG
    if(printCondition) {
        vector<vec2> corners = m_grid->cornersForVoxelWithIndex(voxelIndex);
        cout << "p11=" << corners[0] << ";" << endl;
        cout << "p12=" << corners[1] << ";" << endl;
        cout << "p13=" << corners[2] << ";" << endl;
        cout << "p14=" << corners[3] << ";" << endl;
    }
#endif

    doMove(particleIndex, tau);
    voxelType = m_grid->voxelPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]);
    voxelIndex = m_grid->indexPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]);
#ifdef DSMC_DEBUG
    if(printCondition) cout << "r1=[" << m_particles->x[particleIndex] << ", " << m_particles->y[particleIndex] << "]" << ";" << endl;

    if(printCondition) {
        vector<vec2> corners = m_grid->cornersForVoxelWithIndex(voxelIndex);
        cout << "p21=" << corners[0] << ";" << endl;
        cout << "p22=" << corners[1] << ";" << endl;
        cout << "p23=" << corners[2] << ";" << endl;
        cout << "p24=" << corners[3] << ";" << endl;
    }
#endif
    int count = 0;

    if(voxelType >= +VoxelType::Wall) {
        while(voxelType != +VoxelType::Surface) {
            if(++count > 100) {
                cout << "Warning, we found an infinite loop with molecule " << particleIndex << " at timestep " << m_system->numberOfTimesteps() << " on a wall" << endl;
                exit(1);
            }
#ifdef DSMC_DEBUG
            if(printCondition) cout << "Inside while != surface" << endl;
#endif
            if(voxelType == +VoxelType::Wall) {
                tau /= 2;
                doMove(particleIndex, -tau);
                voxelType = m_grid->voxelPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]);
                voxelIndex = m_grid->indexPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]);
#ifdef DSMC_DEBUG
                if(printCondition) cout << "It is a wall, we moved back to a voxel with index " << voxelIndex << " and type " << int(voxelType) << endl;
#endif
            } else {
                dt -= tau;
#ifdef DSMC_DEBUG
                if(printCondition) cout << "It is a surface, we will recursively call this function" << endl;
#endif
                if(dt > 1e-5 && depth < 10) {
                    moveParticle(particleIndex, dt, depth+1);
                }
                return;
            }
        }

        if(voxelType == +VoxelType::Empty) {
            cout << "Error 1: We hit a wall, but managed to skip the surface voxel. Decrease your timestep." << endl;
            exit(1);
        }

        unsigned int collisionVoxelIndex = m_grid->indexPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]);
#ifdef DSMC_DEBUG
        if(printCondition) cout << "We have now reached a voxel with index " << collisionVoxelIndex << " and type " << int(m_grid->voxelWithIndex(collisionVoxelIndex)) << endl;
#endif
        count = 0;
        while(voxelType == +VoxelType::Surface) {
            if(++count > 100) {
                cout << "Warning, we found an infinite loop with molecule " << particleIndex << " at timestep " << m_system->numberOfTimesteps() << " on a surface" << endl;
                exit(1);
            }

            collisionVoxelIndex = m_grid->indexPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]);
            doMove(particleIndex, -tau); // Move back outside surface voxel
#ifdef DSMC_DEBUG
            if(printCondition) cout << "Moved back with tau=" << -tau << ", now at voxel with index " << m_grid->indexPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]) << " and type " << int(m_grid->voxelPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex])) << endl;
#endif
            tau = m_grid->timeUntilCollision(collisionVoxelIndex, vec2(m_particles->x[particleIndex], m_particles->y[particleIndex]), vec2(m_particles->vx[particleIndex], m_particles->vy[particleIndex])) - 1e-6;
#ifdef DSMC_DEBUG
            if(printCondition) cout << "Collision time is " << tau << endl;
            cout.precision(15);

            if(printCondition) cout << "1: Position before we move is r=[" << m_particles->x[particleIndex] << ", " << m_particles->y[particleIndex] << "]" << endl;
#endif
            doMove(particleIndex, tau); // Move close to surface
#ifdef DSMC_DEBUG
            if(printCondition) cout << "1: Position after we moved is r=[" << m_particles->x[particleIndex] << ", " << m_particles->y[particleIndex] << "]" << endl;
            if(printCondition) cout << "1: Moved to voxel surface, we are now at voxel with index " << m_grid->indexPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]) << " and type " << int(m_grid->voxelPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex])) << endl;
#endif
            voxelType = m_grid->voxelPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]);
        }

        // We're not at the boundary anymore, so we can move over here and do happy colliding
        dt -= tau;

        if(m_grid->voxelWithIndex(collisionVoxelIndex) != +VoxelType::Surface) {
            cout << "Error 2: We hit a wall, but managed to skip the surface voxel. Decrease your timestep." << endl;
            exit(1);
        }

        // Calculate surface collision
        float v_normal = m_sqrtWallTemperatureOverMass*sqrt(-2.0*log(m_random->nextFloat()));
        float v_tangent = m_random->nextGaussian(0, m_sqrtWallTemperatureOverMass);
#ifdef DSMC_DEBUG
        if(printCondition)cout << "Normal: [" << m_grid->normal(collisionVoxelIndex, 0) << ", " << m_grid->normal(collisionVoxelIndex, 1) << "]" << endl;
        if(printCondition)cout << "Tangent: [" << m_grid->tangent(collisionVoxelIndex, 0) << ", " << m_grid->tangent(collisionVoxelIndex, 1) << "]" << endl;
        if(printCondition)cout << "Old velocity: [" << m_particles->vx[particleIndex] << ", " << m_particles->vy[particleIndex] << "]" << endl;
#endif
        m_particles->vx[particleIndex] = v_normal*m_grid->normal(collisionVoxelIndex, 0) + v_tangent*m_grid->tangent(collisionVoxelIndex, 0);
        m_particles->vy[particleIndex] = v_normal*m_grid->normal(collisionVoxelIndex, 1) + v_tangent*m_grid->tangent(collisionVoxelIndex, 1);
#ifdef DSMC_DEBUG
        if(printCondition)cout << "New velocity: [" << m_particles->vx[particleIndex] << ", " << m_particles->vy[particleIndex] << "]" << endl;
        if(printCondition)cout << "dt remaining: " << dt << endl;
        if(printCondition)cout << "We are now at voxel with index " << m_grid->indexPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]) << " and type " << int(m_grid->voxelPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex])) << endl;
        if(printCondition)cout << "Moved to a voxel with index " << m_grid->indexPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex]) << " and type " << int(m_grid->voxelPeriodic(m_particles->x[particleIndex], m_particles->y[particleIndex])) << endl;
#endif
    } else dt=0;


    if(dt > 1e-5 && depth < 10) {
        moveParticle(particleIndex, dt, depth+1);
    }
}

void ParticleMover::moveParticles(double dt)
{
    assert(m_isInitialized && "ParticleMover not initialized.");
    CPElapsedTimer::moveParticles().start();
    Particles *particles = m_system->particles();
    const unsigned int numberOfParticles = particles->numberOfParticles();
    for(unsigned int i=0; i<numberOfParticles; i++) {
        m_particles->x[i] += m_particles->vx[i]*dt;
        m_particles->y[i] += m_particles->vy[i]*dt;
        // moveParticle(i, dt, 0);
    }
    CPElapsedTimer::moveParticles().stop();

    applyPeriodicBoundaryConditions();
}

void ParticleMover::applyPeriodicBoundaryConditions()
{
    CPElapsedTimer::periodicBoundaryConditions().start();
    Particles *particles = m_system->particles();
    vec2 systemSize = m_system->size();

    const unsigned int numberOfParticles = particles->numberOfParticles();
    for(unsigned int i=0; i<numberOfParticles; i++) {
        if(particles->x[i] < 0 && particles->x[i] > -1e-6) particles->x[i] = 0;
        if(particles->y[i] < 0 && particles->y[i] > -1e-6) particles->y[i] = 0;

        if(particles->x[i] >= systemSize[0]) particles->x[i] -= systemSize[0];
        else if(particles->x[i] < 0) particles->x[i] += systemSize[0];

        if(particles->y[i] >= systemSize[1]) particles->y[i] -= systemSize[1];
        else if(particles->y[i] < 0) particles->y[i] += systemSize[1];
    }
    CPElapsedTimer::periodicBoundaryConditions().stop();
}
