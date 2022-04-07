/*!
 * @file IceGrowth.hpp
 *
 * @date Mar 15, 2022
 * @author Tim Spain <timothy.spain@nersc.no>
 */

#ifndef PHYSICS_SRC_INCLUDE_ICEGROWTH_HPP
#define PHYSICS_SRC_INCLUDE_ICEGROWTH_HPP

#include "include/ModelComponent.hpp"
#include "include/Configured.hpp"
#include "include/IVerticalIceGrowth.hpp"
#include "include/ILateralIceSpread.hpp"
#include "include/Time.hpp"

namespace Nextsim {

class IceGrowth : public ModelComponent, public Configured<IceGrowth> {
public:
    IceGrowth()
    {
        registerModule();
        ModelComponent::registerSharedArray(SharedArray::H_ICE, &hice);
        ModelComponent::registerSharedArray(SharedArray::C_ICE, &cice);
        ModelComponent::registerSharedArray(SharedArray::H_SNOW, &hsnow);

        ModelComponent::requestSharedArray(SharedArray::Q_OW, &qow);

        ModelComponent::requestProtectedArray(ProtectedArray::SST, &sst);
        ModelComponent::requestProtectedArray(ProtectedArray::ML_BULK_CP, &mixedLayerBulkHeatCapacity);
        ModelComponent::requestProtectedArray(ProtectedArray::TF, &tf);
    }
    virtual ~IceGrowth() = default;

    void configure() override;
    enum {
        VERTICAL_GROWTH_KEY,
        LATERAL_GROWTH_KEY,
    };

    std::string getName() const override { return "IceGrowth"; }

    void setData(const ModelState&) override {};
    ModelState getState() const override { return ModelState(); }
    ModelState getState(const OutputLevel&) const override { return getState(); }

    std::set<std::string> hFields() const override { return { "updated_hice", "updated_cice", "updated_hsnow" }; }
    std::set<std::string> uFields() const override { return {}; }
    std::set<std::string> vFields() const override { return {}; }
    std::set<std::string> zFields() const override { return {}; }

    void update(const TimestepTime&);

private:
    // Vertical Growth ModelComponent & Module
    std::unique_ptr<IVerticalIceGrowth> iVertical;
    // Lateral Growth ModuleComponent & Module
    std::unique_ptr<ILateralIceSpread> iLateral;

    // Data fields
    // Owned, shared data fields
    HField hice; // Updated true ice thickness, m
    HField cice; // Updated ice concentration
    HField hsnow; // Updated true snow thickness, m

    HField newice; // New ice thickness this timestep, m

    pHField qow; // open water heat flux, from FluxCalculation
    pConstHField mixedLayerBulkHeatCapacity; // J K⁻¹ m⁻², from atmospheric state
    pConstHField sst; // sea surface temperature, ˚C
    pConstHField tf; // ocean freezing point, ˚C

    void newIceFormation(size_t i, const TimestepTime&);
};

} /* namespace Nextsim */

#endif /* PHYSICS_SRC_INCLUDE_ICEGROWTH_HPP */
