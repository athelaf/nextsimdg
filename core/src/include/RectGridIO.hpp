/*!
 * @file RectGridIO.hpp
 *
 * @date Feb 8, 2022
 * @author Tim Spain <timothy.spain@nersc.no>
 */

#ifndef RECTGRIDIO_HPP
#define RECTGRIDIO_HPP

#include "include/RectangularGrid.hpp"

namespace Nextsim {

class RectGridIO : public RectangularGrid::IRectGridIO {
public:
    RectGridIO(RectangularGrid& grid)
        : IRectGridIO(grid)
    {
    }
    virtual ~RectGridIO() = default;

    typedef RectangularGrid::GridDimensions GridDimensions;

#ifdef USE_MPI
    ModelState getModelState(
        const std::string& restartFilePath, const std::string& partitionFilePath) const override;
#else
    ModelState getModelState(const std::string& filePath) override;
#endif // USE_MPI

    // FIXME: add MPI support
    void dumpModelState(const ModelState& state, const ModelMetadata& metadata,
        const std::string& filePath, bool isRestart) const override;

private:
    RectGridIO() = default;
};

} /* namespace Nextsim */

#endif /* RECTGRIDIO_HPP */
