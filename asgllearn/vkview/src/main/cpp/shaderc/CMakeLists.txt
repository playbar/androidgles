
#SPIRV
add_library(SPIRV STATIC
       	third_party/glslang/SPIRV/GlslangToSpv.cpp
       	third_party/glslang/SPIRV/InReadableOrder.cpp
       	third_party/glslang/SPIRV/Logger.cpp
       	third_party/glslang/SPIRV/SPVRemapper.cpp
       	third_party/glslang/SPIRV/SpvBuilder.cpp
       	third_party/glslang/SPIRV/disassemble.cpp
       	third_party/glslang/SPIRV/doc.cpp
            )

target_include_directories(SPIRV PRIVATE
                          ${CMAKE_CURRENT_SOURCE_DIR}/data
                          ${CMAKE_CURRENT_SOURCE_DIR}
                          ${CMAKE_CURRENT_SOURCE_DIR}/third_party/shaderc/include)


#OSDependent
add_library(OSDependent STATIC
            third_party/glslang/glslang/OSDependent/Unix/ossource.cpp
            )

target_include_directories(OSDependent PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glslang/glslang/OSDependent/Unix)


#OGLCompiler
add_library(OGLCompiler STATIC
            third_party/glslang/OGLCompilersDLL/InitializeDll.cpp
            )

target_include_directories(OGLCompiler PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/OGLCompilersDLL)



#HLSL
add_library(HLSL STATIC
            third_party/glslang/hlsl/hlslAttributes.cpp
            third_party/glslang/hlsl/hlslGrammar.cpp
            third_party/glslang/hlsl/hlslOpMap.cpp
            third_party/glslang/hlsl/hlslParseables.cpp
            third_party/glslang/hlsl/hlslParseHelper.cpp
            third_party/glslang/hlsl/hlslScanContext.cpp
            third_party/glslang/hlsl/hlslTokenStream.cpp
            )

target_include_directories(HLSL PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glslang/hlsl )


#glslang
add_library(glslang STATIC
            third_party/glslang/glslang/GenericCodeGen/CodeGen.cpp
            third_party/glslang/glslang/GenericCodeGen/Link.cpp
            third_party/glslang/glslang/MachineIndependent/Constant.cpp
            third_party/glslang/glslang/MachineIndependent/glslang_tab.cpp
            third_party/glslang/glslang/MachineIndependent/InfoSink.cpp
            third_party/glslang/glslang/MachineIndependent/Initialize.cpp
            third_party/glslang/glslang/MachineIndependent/Intermediate.cpp
            third_party/glslang/glslang/MachineIndependent/intermOut.cpp
            third_party/glslang/glslang/MachineIndependent/IntermTraverse.cpp
            third_party/glslang/glslang/MachineIndependent/iomapper.cpp
            third_party/glslang/glslang/MachineIndependent/limits.cpp
            third_party/glslang/glslang/MachineIndependent/linkValidate.cpp
            third_party/glslang/glslang/MachineIndependent/parseConst.cpp
            third_party/glslang/glslang/MachineIndependent/ParseContextBase.cpp
            third_party/glslang/glslang/MachineIndependent/ParseHelper.cpp
            third_party/glslang/glslang/MachineIndependent/PoolAlloc.cpp
            third_party/glslang/glslang/MachineIndependent/propagateNoContraction.cpp
            third_party/glslang/glslang/MachineIndependent/reflection.cpp
            third_party/glslang/glslang/MachineIndependent/RemoveTree.cpp
            third_party/glslang/glslang/MachineIndependent/Scan.cpp
            third_party/glslang/glslang/MachineIndependent/ShaderLang.cpp
            third_party/glslang/glslang/MachineIndependent/SymbolTable.cpp
            third_party/glslang/glslang/MachineIndependent/Versions.cpp
            third_party/glslang/glslang/MachineIndependent/preprocessor/PpAtom.cpp
            third_party/glslang/glslang/MachineIndependent/preprocessor/PpContext.cpp
            third_party/glslang/glslang/MachineIndependent/preprocessor/Pp.cpp
            third_party/glslang/glslang/MachineIndependent/preprocessor/PpScanner.cpp
            third_party/glslang/glslang/MachineIndependent/preprocessor/PpTokens.cpp
            )

target_include_directories(glslang PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glslang/glslang/MachineIndependent )



#SPIRV-Tools
add_library(SPIRV-Tools STATIC
            third_party/spirv-tools/source/assembly_grammar.cpp
            third_party/spirv-tools/source/binary.cpp
            third_party/spirv-tools/source/diagnostic.cpp
            third_party/spirv-tools/source/disassemble.cpp
            third_party/spirv-tools/source/ext_inst.cpp
            third_party/spirv-tools/source/enum_string_mapping.cpp
            third_party/spirv-tools/source/extensions.cpp
            third_party/spirv-tools/source/id_descriptor.cpp
            third_party/spirv-tools/source/libspirv.cpp
            third_party/spirv-tools/source/name_mapper.cpp
            third_party/spirv-tools/source/opcode.cpp
            third_party/spirv-tools/source/operand.cpp
            third_party/spirv-tools/source/parsed_operand.cpp
            third_party/spirv-tools/source/print.cpp
            third_party/spirv-tools/source/software_version.cpp
            third_party/spirv-tools/source/spirv_endian.cpp
            third_party/spirv-tools/source/spirv_target_env.cpp
            third_party/spirv-tools/source/spirv_validator_options.cpp
            third_party/spirv-tools/source/table.cpp
            third_party/spirv-tools/source/text.cpp
            third_party/spirv-tools/source/text_handler.cpp
            third_party/spirv-tools/source/util/bit_stream.cpp
            third_party/spirv-tools/source/util/parse_number.cpp
            third_party/spirv-tools/source/util/string_utils.cpp
            third_party/spirv-tools/source/val/basic_block.cpp
            third_party/spirv-tools/source/val/construct.cpp
            third_party/spirv-tools/source/val/function.cpp
            third_party/spirv-tools/source/val/instruction.cpp
            third_party/spirv-tools/source/val/validation_state.cpp
            third_party/spirv-tools/source/validate.cpp
            third_party/spirv-tools/source/validate_arithmetics.cpp
            third_party/spirv-tools/source/validate_bitwise.cpp
            third_party/spirv-tools/source/validate_capability.cpp
            third_party/spirv-tools/source/validate_cfg.cpp
            third_party/spirv-tools/source/validate_conversion.cpp
            third_party/spirv-tools/source/validate_datarules.cpp
            third_party/spirv-tools/source/validate_decorations.cpp
            third_party/spirv-tools/source/validate_id.cpp
            third_party/spirv-tools/source/validate_instruction.cpp
            third_party/spirv-tools/source/validate_layout.cpp
            third_party/spirv-tools/source/validate_logicals.cpp
            third_party/spirv-tools/source/validate_type_unique.cpp
            third_party/spirv-tools/source/validate_barriers.cpp
            third_party/spirv-tools/source/validate_image.cpp
            third_party/spirv-tools/source/validate_atomics.cpp
            third_party/spirv-tools/source/validate_literals.cpp
            third_party/spirv-tools/source/validate_composites.cpp
            third_party/spirv-tools/source/validate_derivatives.cpp
            third_party/spirv-tools/source/validate_ext_inst.cpp
            third_party/spirv-tools/source/validate_primitives.cpp
            third_party/spirv-tools/source/validate_non_uniform.cpp
            third_party/spirv-tools/source/validate_adjacency.cpp
            third_party/spirv-tools/source/validate_builtins.cpp

            )

target_include_directories(SPIRV-Tools PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spirv-tools/include
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spirv-tools/source
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spirv-tools/external/spirv-headers/include
            )


#SPIRV-Tools-opt
add_library(SPIRV-Tools-opt STATIC
            third_party/spirv-tools/source/opt/aggressive_dead_code_elim_pass.cpp
            third_party/spirv-tools/source/opt/basic_block.cpp
            third_party/spirv-tools/source/opt/block_merge_pass.cpp
            third_party/spirv-tools/source/opt/build_module.cpp
            third_party/spirv-tools/source/opt/cfg_cleanup_pass.cpp
            third_party/spirv-tools/source/opt/compact_ids_pass.cpp
            third_party/spirv-tools/source/opt/common_uniform_elim_pass.cpp
            third_party/spirv-tools/source/opt/dead_branch_elim_pass.cpp
            third_party/spirv-tools/source/opt/dead_variable_elimination.cpp
            third_party/spirv-tools/source/opt/decoration_manager.cpp
            third_party/spirv-tools/source/opt/def_use_manager.cpp
            third_party/spirv-tools/source/opt/eliminate_dead_constant_pass.cpp
            third_party/spirv-tools/source/opt/eliminate_dead_functions_pass.cpp
            third_party/spirv-tools/source/opt/flatten_decoration_pass.cpp
            third_party/spirv-tools/source/opt/fold.cpp
            third_party/spirv-tools/source/opt/fold_spec_constant_op_and_composite_pass.cpp
            third_party/spirv-tools/source/opt/freeze_spec_constant_value_pass.cpp
            third_party/spirv-tools/source/opt/function.cpp
            third_party/spirv-tools/source/opt/inline_pass.cpp
            third_party/spirv-tools/source/opt/inline_exhaustive_pass.cpp
            third_party/spirv-tools/source/opt/inline_opaque_pass.cpp
            third_party/spirv-tools/source/opt/insert_extract_elim.cpp
            third_party/spirv-tools/source/opt/instruction.cpp
            third_party/spirv-tools/source/opt/instruction_list.cpp
            third_party/spirv-tools/source/opt/ir_loader.cpp
            third_party/spirv-tools/source/opt/local_access_chain_convert_pass.cpp
            third_party/spirv-tools/source/opt/local_single_block_elim_pass.cpp
            third_party/spirv-tools/source/opt/local_single_store_elim_pass.cpp
            third_party/spirv-tools/source/opt/local_ssa_elim_pass.cpp
            third_party/spirv-tools/source/opt/mem_pass.cpp
            third_party/spirv-tools/source/opt/module.cpp
            third_party/spirv-tools/source/opt/optimizer.cpp
            third_party/spirv-tools/source/opt/pass.cpp
            third_party/spirv-tools/source/opt/pass_manager.cpp
            third_party/spirv-tools/source/opt/remove_duplicates_pass.cpp
            third_party/spirv-tools/source/opt/set_spec_constant_default_value_pass.cpp
            third_party/spirv-tools/source/opt/strength_reduction_pass.cpp
            third_party/spirv-tools/source/opt/strip_debug_info_pass.cpp
            third_party/spirv-tools/source/opt/type_manager.cpp
            third_party/spirv-tools/source/opt/types.cpp
            third_party/spirv-tools/source/opt/unify_const_pass.cpp
            third_party/spirv-tools/source/opt/constants.cpp
            third_party/spirv-tools/source/opt/ir_context.cpp
            third_party/spirv-tools/source/opt/strip_reflect_info_pass.cpp
            third_party/spirv-tools/source/opt/local_redundancy_elimination.cpp
            third_party/spirv-tools/source/opt/loop_fusion_pass.cpp
            third_party/spirv-tools/source/opt/licm_pass.cpp
            third_party/spirv-tools/source/opt/loop_unswitch_pass.cpp
            third_party/spirv-tools/source/opt/scalar_replacement_pass.cpp
            third_party/spirv-tools/source/opt/private_to_local_pass.cpp
            third_party/spirv-tools/source/opt/ccp_pass.cpp
            third_party/spirv-tools/source/opt/dead_insert_elim_pass.cpp
            third_party/spirv-tools/source/opt/merge_return_pass.cpp
            third_party/spirv-tools/source/opt/loop_peeling.cpp
            third_party/spirv-tools/source/opt/loop_fission.cpp
            third_party/spirv-tools/source/opt/loop_descriptor.cpp
            third_party/spirv-tools/source/opt/workaround1209.cpp
            third_party/spirv-tools/source/opt/redundancy_elimination.cpp
            third_party/spirv-tools/source/opt/if_conversion.cpp
            third_party/spirv-tools/source/opt/replace_invalid_opc.cpp
            third_party/spirv-tools/source/opt/dominator_analysis.cpp
            third_party/spirv-tools/source/opt/loop_unroller.cpp
            third_party/spirv-tools/source/opt/ssa_rewrite_pass.cpp
            third_party/spirv-tools/source/opt/reduce_load_size.cpp
            third_party/spirv-tools/source/opt/value_number_table.cpp
            third_party/spirv-tools/source/opt/simplification_pass.cpp
            third_party/spirv-tools/source/opt/composite.cpp
            third_party/spirv-tools/source/opt/const_folding_rules.cpp
            third_party/spirv-tools/source/opt/cfg.cpp
            third_party/spirv-tools/source/opt/copy_prop_arrays.cpp
            third_party/spirv-tools/source/opt/vector_dce.cpp
            third_party/spirv-tools/source/opt/scalar_analysis.cpp
            third_party/spirv-tools/source/opt/loop_fusion.cpp
            third_party/spirv-tools/source/opt/register_pressure.cpp
            third_party/spirv-tools/source/opt/feature_manager.cpp
            third_party/spirv-tools/source/opt/loop_utils.cpp
            third_party/spirv-tools/source/opt/dominator_tree.cpp
            third_party/spirv-tools/source/opt/propagator.cpp
            third_party/spirv-tools/source/opt/scalar_analysis_simplification.cpp
            third_party/spirv-tools/source/opt/loop_dependence.cpp
            third_party/spirv-tools/source/opt/folding_rules.cpp
            third_party/spirv-tools/source/opt/loop_dependence_helpers.cpp
            third_party/spirv-tools/source/util/bit_vector.cpp
            )

target_include_directories(SPIRV-Tools-opt PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spirv-tools/include
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spirv-tools/source
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spirv-tools/external/spirv-headers/include)



#shaderc_util
add_library(shaderc_util STATIC
            libshaderc_util/src/compiler.cc
            libshaderc_util/src/file_finder.cc
            libshaderc_util/src/io.cc
            libshaderc_util/src/message.cc
            libshaderc_util/src/resources.cc
            libshaderc_util/src/shader_stage.cc
            libshaderc_util/src/spirv_tools_wrapper.cc
            libshaderc_util/src/version_profile.cc
            )

target_include_directories(shaderc_util PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/libshaderc_util/include
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glslang
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spirv-tools/include
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spirv-tools/source
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spirv-tools/external/spirv-headers/include)


#shaderc
add_library(shaderc STATIC
            libshaderc/src/shaderc.cc
            )

target_include_directories(shaderc PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/libshaderc/include
            ${CMAKE_CURRENT_SOURCE_DIR}/libshaderc_util/include
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glslang
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spirv-tools/include
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spirv-tools/source
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spirv-tools/external/spirv-headers/include)
