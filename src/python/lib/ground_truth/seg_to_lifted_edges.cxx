#include <pybind11/pybind11.h>
#include <iostream>
#include <sstream>
#include <pybind11/numpy.h>

#include "nifty/python/converter.hxx"

#include "nifty/ground_truth/seg_to_lifted_edges.hxx"

namespace py = pybind11;


namespace nifty{
namespace ground_truth{



    void exportSeg2dToLiftedEdges(py::module & groundTruthModule){

        groundTruthModule.def("seg2dToLiftedEdges",
        [](
            marray::PyView<uint32_t, 2 , false>   seg,
            marray::PyView<int32_t, 2 ,  false>   edges
        ){
            NIFTY_CHECK_OP(edges.shape(1), == , 2, "edges must be |N| x 2")
            marray::PyView<uint8_t> out({
                size_t(seg.shape(0)),
                size_t(seg.shape(1)),
                size_t(edges.shape(0))
            }); 
            
            std::vector<std::array<int32_t, 2> > e(edges.shape(0));
            for(size_t i=0; i<e.size(); ++i){
                e[i][0] = edges(i, 0);
                e[i][1] = edges(i, 1);
            }

            seg2dToLiftedEdges(seg, e, out);

            return out;
        },
            py::arg("segmentation"),
            py::arg("edges")
        );
    }

    void exportSeg3dToLiftedEdges(py::module & groundTruthModule){

        groundTruthModule.def("seg2dToLiftedEdges",
        [](
            marray::PyView<uint32_t, 2 , false>   seg,
            marray::PyView<int32_t, 2 ,  false>   edges,
            int32_t z
        ){
            NIFTY_CHECK_OP(edges.shape(1), == , 3, "edges must be |N| x 3");

            marray::PyView<uint8_t> out({
                size_t(seg.shape(0)),
                size_t(seg.shape(1)),
                size_t(edges.shape(0))
            }); 
            
            std::vector<std::array<int32_t, 3> > e(edges.shape(0));
            for(size_t i=0; i<e.size(); ++i){
                e[i][0] = edges(i, 0);
                e[i][1] = edges(i, 1);
                e[i][2] = edges(i, 2);
            }

            seg3dToLiftedEdges(seg, e, z, out);

            return out;
        },
            py::arg("segmentation"),
            py::arg("edges"),
            py::arg("z")
        );
    }


    void exportSeg3dToCremiZ5Edges(py::module & groundTruthModule){

        groundTruthModule.def("seg3dToCremiZ5Edges",
        [](
            marray::PyView<uint32_t, 2 , false>   seg,
            marray::PyView<int32_t, 2 ,  false>   edges
        ){
            NIFTY_CHECK_OP(edges.shape(1), == , 4, "edges must be |N| x 4");

            marray::PyView<uint8_t> out({
                size_t(seg.shape(0)),
                size_t(seg.shape(1)),
                size_t(edges.shape(0))
            }); 
            
            std::vector<std::array<int32_t, 4> > e(edges.shape(0));
            for(size_t i=0; i<e.size(); ++i){
                e[i][0] = edges(i, 0);
                e[i][1] = edges(i, 1);
                e[i][2] = edges(i, 2);
                e[i][2] = edges(i, 3);
            }

            seg3dToCremiZ5Edges(seg, e, out);

            return out;
        },
            py::arg("segmentation"),
            py::arg("edges")
        );
    }

    //<class T_SEG>
    //void cremiZ5Edges

    void exportSegToLiftedEdges(py::module & groundTruthModule){
        exportSeg2dToLiftedEdges(groundTruthModule);
        exportSeg3dToLiftedEdges(groundTruthModule);
        exportSeg3dToCremiZ5Edges(groundTruthModule);
    }
}
}