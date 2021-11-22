/*!
 * @file NextsimPhysics_test.cpp
 *
 * @date Sep 9, 2021
 * @author Tim Spain <timothy.spain@nersc.no>
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

#include "include/ElementData.hpp"
#include "include/NextsimPhysics.hpp"
#include "include/constants.hpp"
namespace Nextsim {

TEST_CASE("Outgoing LW (OW)", "[NextsimPhysics]")
{
    ElementData<NextsimPhysics> data;

    const double t = 280.; // kelvin
    data = PrognosticData::generate(0., 0., celsius(t), 0., 0, { 0., 0, 0 });

    // Configure as NextsimPhysics, as the only subclass of Configured.
    data.configure();

    NextsimPhysics::heatFluxOpenWater(data, data, data, data);

    double target = PhysicalConstants::sigma * t * t * t * t;

    REQUIRE(data.QLongwaveOpenWater() == target);
}

TEST_CASE("Minimum ice & i0", "[NextsimPhysics]")
{
    Configurator::clearStreams();

    // Non-default values for the minimum concentration and thickness and I0
    const double minConc = 2e-12;
    const double minThck = 0.02;
    const double i0 = 0.18;

    std::stringstream config;
    config << "[nextsim_thermo]" << std::endl;
    config << "min_conc = " << minConc << std::endl;
    config << "min_thick = " << minThck << std::endl;
    config << "I_0 = " << i0 << std::endl;

    std::unique_ptr<std::istream> pcstream(new std::stringstream(config.str()));
    Configurator::addStream(std::move(pcstream));

    NextsimPhysics nsphys;
    nsphys.configure();

    REQUIRE(NextsimPhysics::minimumIceConcentration() == minConc);
    REQUIRE(NextsimPhysics::minimumIceThickness() == minThck);
    REQUIRE(NextsimPhysics::i0() == i0);
}

TEST_CASE("Update derived data", "[NextsimPhysics]")
{
    ElementData<NextsimPhysics> data;

    double tair = -3;
    double tdew = 0.1;
    double pair = 100000; // Slightly low pressure
    double sst = -1;
    double sss = 32; // PSU
    std::array<double, N_ICE_TEMPERATURES> tice = { -2., -2, -2 };
    double hice = 0.1;
    double cice = 0.5;

    data = PrognosticData::generate(hice, cice, sst, sss, 0., tice);
    data.airTemperature() = tair;
    data.dewPoint2m() = tdew;
    data.airPressure() = pair;

    NextsimPhysics::updateDerivedData(data, data, data, data);

    REQUIRE(1.2895 == Approx(data.airDensity()).epsilon(1e-4));
    CHECK(0.00385326 == Approx(data.specificHumidityAir()).epsilon(1e-4));
    CHECK(0.00349446 == Approx(data.specificHumidityWater()).epsilon(1e-4));
    CHECK(0.00323958 == Approx(data.specificHumidityIce()).epsilon(1e-4));
}
} /* namespace Nextsim */
