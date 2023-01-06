//$file${include::qep.hpp} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: qpcpp.qm
// File:  ${include::qep.hpp}
//
// This code has been generated by QM 5.2.4 <www.state-machine.com/qm>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// This code is covered by the following QP license:
// License #    : LicenseRef-QL-dual
// Issued to    : Any user of the QP/C++ real-time embedded framework
// Framework(s) : qpcpp
// Support ends : 2023-12-31
// License scope:
//
// Copyright (C) 2005 Quantum Leaps, LLC <state-machine.com>.
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open source GNU
// General Public License version 3 (or any later version), or alternatively,
// under the terms of one of the closed source Quantum Leaps commercial
// licenses.
//
// The terms of the open source GNU General Public License version 3
// can be found at: <www.gnu.org/licenses/gpl-3.0>
//
// The terms of the closed source Quantum Leaps commercial licenses
// can be found at: <www.state-machine.com/licensing>
//
// Redistributions in source code must retain this top-level comment block.
// Plagiarizing this software to sidestep the license obligations is illegal.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//
//$endhead${include::qep.hpp} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//! @file
//! @brief QEP/C++ platform-independent public interface.
//!
//! @tr{RQP001} @tr{RQP101}

#ifndef QP_INC_QEP_HPP_
#define QP_INC_QEP_HPP_

//============================================================================
//! The current QP version as an unsigned number
//
// @details
// ::QP_VERSION is a decimal constant, where XX is a 1-digit or 2-digit
// major version number, Y is a 1-digit minor version number, and Z is
// a 1-digit release number.
//
#define QP_VERSION 720U

//! The current QP version as a zero terminated string literal.
//
// @details
// ::QP_VERSION_STR is of the form "XX.Y.Z", where XX is a 1-or 2-digit
// major version number, Y is a 1-digit minor version number, and Z is
// a 1-digit release number.
//
#define QP_VERSION_STR "7.2.0"

//! Encrypted  current QP release (7.2.0) and date (2023-01-06)
#define QP_RELEASE 0x76D8998FU

//============================================================================
// Global namespace...
//$declare${glob-types} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${glob-types::int_t} .......................................................
//! alias for line numbers in assertions and return from QF::run()
using int_t  = int;

//${glob-types::enum_t} ......................................................
//! alias for enumerations used for event signals
using enum_t  = int;

//${glob-types::float32_t} ...................................................
//! alias for 32-bit IEEE 754 floating point numbers
//!
//! @note
//! QP does not use floating-point types anywhere in the internal
//! implementation, except in QS software tracing, where utilities for
//! output of floating-point numbers are provided for application-specific
//! trace records.
using float32_t  = float;

//${glob-types::float64_t} ...................................................
//! alias for 64-bit IEEE 754 floating point numbers
//!
//! @note
//! QP does not use floating-point types anywhere in the internal
//! implementation, except in QS software tracing, where utilities for
//! output of floating-point numbers are provided for application-specific
//! trace records.
using float64_t  = double;
//$enddecl${glob-types} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$declare${QEP-config} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${QEP-config::Q_SIGNAL_SIZE} ...............................................
#ifndef Q_SIGNAL_SIZE
//! The size (in bytes) of the signal of an event. Valid values:
//! 1U, 2U, or 4U; default 2U
//!
//! @details
//! This macro can be defined in the QEP port file (qep_port.hpp) to
//! configure the QP::QSignal type. When the macro is not defined, the
//! default of 2 bytes is applied.
#define Q_SIGNAL_SIZE 2U
#endif // ndef Q_SIGNAL_SIZE
//$enddecl${QEP-config} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//============================================================================
//$declare${QEP} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace QP {

//${QEP::versionStr[]} .......................................................
//! the current QP version number string based on QP_VERSION_STR
constexpr char const versionStr[] {QP_VERSION_STR};

//${QEP::QSignal} ............................................................
#if (Q_SIGNAL_SIZE == 2U)
//! QSignal represents the signal of an event
//!
//! @details
//! The relationship between an event and a signal is as follows. A signal
//! in UML is the specification of an asynchronous stimulus that triggers
//! reactions, and as such is an essential part of an event. (The signal
//! conveys the type of the occurrence--what happened?) However, an event
//! can also contain additional quantitative information about the
//! occurrence in form of event parameters.
using QSignal = std::uint16_t;
#endif //  (Q_SIGNAL_SIZE == 2U)

//${QEP::QSignal} ............................................................
#if (Q_SIGNAL_SIZE == 1U)
using QSignal = std::uint8_t;
#endif //  (Q_SIGNAL_SIZE == 1U)

//${QEP::QSignal} ............................................................
#if (Q_SIGNAL_SIZE == 4U)
using QSignal = std::uint32_t;
#endif //  (Q_SIGNAL_SIZE == 4U)

//${QEP::QEvt} ...............................................................
//! Event class
//!
//! @details
//! QP::QEvt represents events without parameters and serves as the
//! base class for derivation of events with parameters.
//!
//! @note
//! When #Q_EVT_CTOR and #Q_EVT_VIRTUAL are NOT defined, the QP::QEvt is
//! a POD (Plain Old Data). Otherwise, it is a class with constructors
//! and virtual destructor.
//!
//! @usage
//! The following example illustrates how to add an event parameter by
//! inheriting from the QP::QEvt class.
//! @include qep_qevt.cpp
class QEvt {
public:

    //! signal of the event instance
    //! @tr{RQP002}
    QSignal sig;

    //! pool ID (0 for static, immutable event)
    //! @tr{RQP003}
    std::uint8_t poolId_;

    //! reference counter (only used for dynamic, mutable events)
    //! @tr{RQP003}
    std::uint8_t volatile refCtr_;

public:

#ifdef Q_EVT_CTOR
    //! QP::QEvt constructor when the macro #Q_EVT_CTOR is defined
    explicit QEvt(QSignal s) noexcept
    : sig(s)
      // poolId_/refCtr_ intentionally uninitialized
    {}
#endif // def Q_EVT_CTOR

#ifdef Q_EVT_CTOR
    //! QP::QEvt constructor (overload for static, immutable events)
    constexpr QEvt(
        QSignal s,
        std::uint8_t /* dummy */) noexcept
    : sig(s),
        poolId_(0U),
        refCtr_(0U)
    {}
#endif // def Q_EVT_CTOR

#ifdef Q_EVT_VIRTUAL
    //! QP::QEvt virtual destructor when the macro #Q_EVT_VIRTUAL is defined
    virtual ~QEvt() noexcept {
        // empty
    }
#endif // def Q_EVT_VIRTUAL
}; // class QEvt

//${QEP::QState} .............................................................
//! Type returned from state-handler functions
using QState = std::uint_fast8_t;

//${QEP::QStateHandler} ......................................................
//! Pointer to state-handler function
using QStateHandler = QState (*)(void * const me, QEvt const * const e);

//${QEP::QActionHandler} .....................................................
//! Pointer to an action-handler function
using QActionHandler = QState (*)(void * const me);

//${QEP::QXThread} ...........................................................
//! forward declaration
class QXThread;

//${QEP::QXThreadHandler} ....................................................
//! Pointer to an extended thread-handler function
using QXThreadHandler = void (*)(QXThread * const me);

//${QEP::QMState} ............................................................
//! State object for the QP::QMsm class (QM State Machine).
//!
//! @details
//! This class groups together the attributes of a QP::QMsm state, such as
//! the parent state (state nesting), the associated state handler function
//! and the exit action handler function. These attributes are used inside
//! the QP::QMsm::dispatch() and QP::QMsm::init() functions.
//!
//! @attention
//! The QP::QMState class is only intended for the QM code generator and
//! should not be used in hand-crafted code.
struct QMState {
    QMState const * superstate;       //!< superstate of this state
    QStateHandler const stateHandler; //!< state handler function
    QActionHandler const entryAction; //!< entry action handler function
    QActionHandler const exitAction;  //!< exit action handler function
    QActionHandler const initAction;  //!< init action handler function
};

//${QEP::QMTranActTable} .....................................................
//! Transition-Action Table for the QP::QMsm State Machine.
struct QMTranActTable {
    QMState  const * target;     //!< target of the transition
    QActionHandler const act[1]; //!< array of actions
};

//${QEP::QHsmAttr} ...........................................................
//! Attribute of for the QP::QHsm class (Hierarchical State Machine)
//!
//! @details
//! This union represents possible values stored in the 'state' and 'temp'
//! attributes of the QP::QHsm class.
union QHsmAttr {
    QStateHandler   fun;          //!< pointer to a state handler function
    QActionHandler  act;          //!< pointer to an action-handler function
    QXThreadHandler thr;          //!< pointer to an thread-handler function
    QMState         const *obj;   //!< pointer to QMState object
    QMTranActTable  const *tatbl; //!< transition-action table
};

//${QEP::Q_USER_SIG} .........................................................
//! Type returned from state-handler functions
constexpr enum_t  Q_USER_SIG {4};

//${QEP::QHsm} ...............................................................
//! Hierarchical State Machine abstract base class (ABC)
//!
//! @details
//! QP::QHsm represents a Hierarchical State Machine (HSM) with full support
//! for hierarchical nesting of states, entry/exit actions, and initial
//! transitions in any composite state. QHsm inherits QP::QMsm without adding
//! new attributes, so it takes the same amount of RAM as QP::QMsm.<br>
//!
//! QP::QHsm is also the base class for the QP::QMsm state machine, which
//! provides better efficiency, but requires the use of the QM modeling tool
//! to generate code.
//!
//! @note
//! QP::QHsm is not intended to be instantiated directly, but rather serves as
//! the base class for derivation of state machines in the application code.
//!
//! @usage
//! The following example illustrates how to derive a state machine class
//! from QP::QHsm.
//! @include qep_qhsm.cpp
class QHsm {
private:

    //! current active state (the state-variable)
    QHsmAttr m_state;

    //! temporary: transition chain, target state, etc.
    QHsmAttr m_temp;

public:

    //! Maximum nesting depth of states in HSM
    static constexpr std::int_fast8_t MAX_NEST_DEPTH_{6};

private:

    // friends...
    friend class QMsm;
    friend class QActive;
    friend class QMActive;
    friend class QXThread;
    friend class QXMutex;
    friend class QXSemaphore;

#ifdef Q_UTEST
    friend class QHsmDummy;
#endif // def Q_UTEST

#ifdef Q_UTEST
    friend class QActiveDummy;
#endif // def Q_UTEST

public:

    //! Reserved signals by the HSM-style state machine
    //! implementation strategy.
    enum ReservedSig : QSignal {
        Q_EMPTY_SIG,  //!< signal to execute the default case
        Q_ENTRY_SIG,  //!< signal for entry actions
        Q_EXIT_SIG,   //!< signal for exit actions
        Q_INIT_SIG    //!< signal for nested initial transitions
    };

    //! All possible return values from state-handlers
    enum QStateRet : std::uint_fast8_t {
        // unhandled and need to "bubble up"
        Q_RET_SUPER,     //!< event passed to superstate to handle
        Q_RET_SUPER_SUB, //!< event passed to submachine superstate
        Q_RET_UNHANDLED, //!< event unhandled due to a guard

        // handled and do not need to "bubble up"
        Q_RET_HANDLED,   //!< event handled (internal transition)
        Q_RET_IGNORED,   //!< event silently ignored (bubbled up to top)

        // entry/exit
        Q_RET_ENTRY,     //!< state entry action executed
        Q_RET_EXIT,      //!< state exit  action executed

        // no side effects
        Q_RET_NULL,      //!< return value without any effect

        // transitions need to execute transition-action table in QP::QMsm
        Q_RET_TRAN,      //!< regular transition
        Q_RET_TRAN_INIT, //!< initial transition in a state or submachine
        Q_RET_TRAN_EP,   //!< entry-point transition into a submachine

        // transitions that additionally clobber QHsm.m_state
        Q_RET_TRAN_HIST, //!< transition to history of a given state
        Q_RET_TRAN_XP    //!< exit-point transition out of a submachine
    };

protected:

    //! protected constructor of QHsm
    explicit QHsm(QStateHandler const initial) noexcept;

public:

#ifdef Q_HSM_XTOR
    //! virtual destructor
    virtual ~QHsm() noexcept {
        // empty
    }
#endif // def Q_HSM_XTOR

    //! Executes the top-most initial transition in QP::QHsm
    //!
    //! @details
    //! Executes the top-most initial transition in a HSM.
    //!
    //! @param[in] e   pointer to an extra parameter (might be NULL)
    //! @param[in] qs_id QS-id of this state machine (for QS local filter)
    //!
    //! @note
    //! Must be called exactly __once__ before the QP::QHsm::dispatch().
    //!
    //! @tr{RQP103} @tr{RQP120I} @tr{RQP120D}
    virtual void init(
        void const * const e,
        std::uint_fast8_t const qs_id);

    //! overloaded init(qs_id)
    //!
    //! @details
    //! Executes the top-most initial transition in a HSM (overloaded).
    //!
    //! @param[in] qs_id   QS-id of this state machine (for QS local filter)
    //!
    //! @attention
    //! QHsm::init() must be called exactly **once** before
    //! QHsm::dispatch()
    virtual void init(std::uint_fast8_t const qs_id) {
        init(nullptr, qs_id);
    }

    //! Dispatches an event to QP::QHsm
    //!
    //! @details
    //! Dispatches an event for processing to a hierarchical state machine.
    //! The event dispatching represents one run-to-completion (RTC) step.
    //!
    //! @param[in] e     pointer to the event to be dispatched to the HSM
    //! @param[in] qs_id QS-id of this state machine (for QS local filter)
    //!
    //! @attention
    //! This state machine must be initialized by calling QP::QHsm::init()
    //! exactly **once** before calling QP::QHsm::dispatch().
    //!
    //! @tr{RQP103}
    //! @tr{RQP120A} @tr{RQP120B} @tr{RQP120C} @tr{RQP120D} @tr{RQP120E}
    virtual void dispatch(
        QEvt const * const e,
        std::uint_fast8_t const qs_id);

    //! The top-state handler
    //!
    //! @details
    //! The QHsm::top() state handler is the ultimate root of state
    //! hierarchy in all HSMs derived from QP::QHsm.
    //!
    //! @param[in] me pointer to the HSM instance
    //! @param[in] e  pointer to the event to be dispatched to the HSM
    //!
    //! @returns
    //! Always returns #Q_RET_IGNORED, which means that the top state ignores
    //! all events.
    //!
    //! @note
    //! The parameters to this state handler are not used. They are provided
    //! for conformance with the state-handler function signature
    //! QP::QStateHandler.
    //!
    //! @tr{RQP103} @tr{RQP120T}
    static QState top(
        void * const me,
        QEvt const * const e) noexcept;

    //! Obtain the current state (state handler function)
    //!
    //! @note used in the QM code generation
    QStateHandler state() const noexcept {
        return m_state.fun;
    }

#ifdef Q_SPY
    //! Get the current state handler of the QP::QHsm
    virtual QStateHandler getStateHandler() noexcept {
        return m_state.fun;
    }
#endif // def Q_SPY

    //! Tests if a given state is part of the current active state
    //! configuration
    //!
    //! @details
    //! Tests if a state machine derived from QHsm is-in a given state.
    //!
    //! @note
    //! For a HSM, to "be in a state" means also to be in a superstate of
    //! of the state.
    //!
    //! @param[in] s pointer to the state-handler function to be tested
    //!
    //! @returns
    //! 'true' if the HSM is in the `state` and 'false' otherwise
    //!
    //! @tr{RQP103}
    //! @tr{RQP120S}
    bool isIn(QStateHandler const s) noexcept;

    //! Obtain the current active child state of a given parent
    //!
    //! @note used in the QM code generation
    QStateHandler childState(QStateHandler const parent) noexcept;

protected:

    //! Helper function to specify a state transition
    QState tran(QStateHandler const target) noexcept {
        m_temp.fun = target;
        return Q_RET_TRAN;
    }

    //! Helper function to specify a transition to history
    QState tran_hist(QStateHandler const hist) noexcept {
        m_temp.fun = hist;
        return Q_RET_TRAN_HIST;
    }

    //! Helper function to specify the superstate of a given state
    QState super(QStateHandler const superstate) noexcept {
        m_temp.fun = superstate;
        return Q_RET_SUPER;
    }

    //! Helper function to specify a regular state transition
    //! in a QM state-handler
    QState qm_tran(void const * const tatbl) noexcept {
        m_temp.tatbl = static_cast<QP::QMTranActTable const *>(tatbl);
        return Q_RET_TRAN;
    }

    //! Helper function to specify an initial state transition
    //! in a QM state-handler
    QState qm_tran_init(void const * const tatbl) noexcept {
        m_temp.tatbl = static_cast<QP::QMTranActTable const *>(tatbl);
        return Q_RET_TRAN_INIT;
    }

    //! Helper function to specifiy a transition to history
    //! in  a QM state-handler
    QState qm_tran_hist(
        QMState const * const hist,
        void const * const tatbl) noexcept
    {
        m_state.obj  = hist;
        m_temp.tatbl = static_cast<QP::QMTranActTable const *>(tatbl);
        return Q_RET_TRAN_HIST;
    }

    //! Helper function to specify a transition to an entry point
    //! to a submachine state in a QM state-handler
    QState qm_tran_ep(void const * const tatbl) noexcept {
        m_temp.tatbl = static_cast<QP::QMTranActTable const *>(tatbl);
        return Q_RET_TRAN_EP;
    }

    //! Helper function to specify a transition to an exit point
    //! from a submachine state in a QM state-handler
    QState qm_tran_xp(
        QActionHandler const xp,
        void const * const tatbl) noexcept
    {
        m_state.act = xp;
        m_temp.tatbl = static_cast<QP::QMTranActTable const *>(tatbl);
        return Q_RET_TRAN_XP;
    }

#ifdef Q_SPY
    //! Helper function to specify a state entry in a QM state-handler
    QState qm_entry(QMState const * const s) noexcept {
        m_temp.obj = s;
        return Q_RET_ENTRY;
    }
#endif // def Q_SPY

#ifndef Q_SPY
    //! Helper function to specify a state entry in a QM state-handler
    QState qm_entry(QMState const * const s) noexcept {
        static_cast<void>(s); // unused parameter
        return Q_RET_ENTRY;
    }
#endif // ndef Q_SPY

#ifdef Q_SPY
    //! Helper function to specify a state exit in a QM state-handler
    QState qm_exit(QMState const * const s) noexcept {
        m_temp.obj = s;
        return Q_RET_EXIT;
    }
#endif // def Q_SPY

#ifndef Q_SPY
    //! Helper function to specify a state exit in a QM state-handler
    QState qm_exit(QMState const * const s) noexcept {
        static_cast<void>(s); // unused parameter
        return Q_RET_EXIT;
    }
#endif // ndef Q_SPY

    //! Helper function to specify a submachine exit in a QM state-handler
    QState qm_sm_exit(QMState const * const s) noexcept {
        m_temp.obj = s;
        return Q_RET_EXIT;
    }

    //! Helper function to call in a QM state-handler when it passes
    //! the event to the host submachine state to handle an event.
    QState qm_super_sub(QMState const * const s) noexcept {
        m_temp.obj = s;
        return Q_RET_SUPER_SUB;
    }

private:

    //! @details
    //! helper function to execute transition sequence in a hierarchical
    //! state machine (HSM).
    //!
    //! @param[in,out] path array of pointers to state-handler functions
    //!                     to execute the entry actions
    //! @param[in]  qs_id   QS-id of this state machine (for QS local filter)
    //!
    //! @returns
    //! the depth of the entry path stored in the `path` parameter.
    //!
    //! @tr{RQP103}
    //! @tr{RQP120E} @tr{RQP120F}
    std::int_fast8_t hsm_tran(
        QStateHandler (&path)[MAX_NEST_DEPTH_],
        std::uint_fast8_t const qs_id);
}; // class QHsm

//${QEP::QMsm} ...............................................................
//! QM State Machine implementation strategy
//!
//! @details
//! QP::QMsm (QM State Machine) provides a more efficient state machine
//! implementation strategy than QHsm, but requires the use of the QM
//! modeling tool, but are the fastest and need the least run-time
//! support (the smallest event-processor taking up the least code space).
//!
//! @note
//! QP::QMsm is not intended to be instantiated directly, but rather serves as
//! the base class for derivation of state machines in the application code.
//!
//! @usage
//! The following example illustrates how to derive a state machine class
//! from QP::QMsm:
//! @include qep_qmsm.cpp
class QMsm : public QP::QHsm {
private:

    //! the top state object for the QP::QMsm
    static QMState const msm_top_s;

    // friends...
    friend class QMActive;

public:

    //! maximum depth of implemented entry levels for transitions to history
    static constexpr std::int_fast8_t MAX_ENTRY_DEPTH_{4};

public:

    //! Performs the second step of SM initialization by triggering
    //! the top-most initial transition.
    //!
    //! @details
    //! Executes the top-most initial transition in a MSM.
    //!
    //! @param[in] e   pointer to an extra parameter (might be nullptr)
    //! @param[in]     qs_id QS-id of this state machine (for QS local filter)
    //!
    //! @attention
    //! QMsm::init() must be called exactly **once** before QMsm::dispatch()
    void init(
        void const * const e,
        std::uint_fast8_t const qs_id) override;

protected:

    //! Protected constructor
    //! @details
    //! Performs the first step of initialization by assigning the initial
    //! pseudostate to the currently active state of the state machine.
    //!
    //! @param[in] initial  the top-most initial transition for the MSM.
    //!
    //! @note
    //! The constructor is protected to prevent direct instantiating of the
    //! QP::QMsm objects. This class is intended for subclassing only.
    //!
    //! @sa
    //! The QP::QMsm example illustrates how to use the QMsm constructor
    //! in the constructor initializer list of the derived state machines.
    explicit QMsm(QStateHandler const initial) noexcept
      : QHsm(initial)
    {
        m_state.obj = &msm_top_s;
        m_temp.fun  = initial;
    }

public:

    //! overloaded init(qs_id)
    //!
    //! @details
    //! Executes the top-most initial transition in a MSM (overloaded).
    //!
    //! @param[in]     qs_id QS-id of this state machine (for QS local filter)
    //!
    //! @attention
    //! QMsm::init() must be called exactly **once** before QMsm::dispatch()
    void init(std::uint_fast8_t const qs_id) override {
        QMsm::init(nullptr, qs_id);
    }

    //! Dispatches an event to a MSM
    //!
    //! @details
    //! Dispatches an event for processing to a meta state machine (MSM).
    //! The processing of an event represents one run-to-completion (RTC) step.
    //!
    //! @param[in] e  pointer to the event to be dispatched to the MSM
    //! @param[in] qs_id QS-id of this state machine (for QS local filter)
    //!
    //! @note
    //! Must be called after QMsm::init().
    void dispatch(
        QEvt const * const e,
        std::uint_fast8_t const qs_id) override;

    //! Tests if a given state is part of the active state configuration
    //!
    //! @details
    //! Tests if a state machine derived from QMsm is-in a given state.
    //!
    //! @note
    //! For a MSM, to "be-in" a state means also to "be-in" a superstate of
    //! of the state.
    //!
    //! @param[in] st  pointer to the QMState object that corresponds to the
    //!                tested state.
    //! @returns
    //! 'true' if the MSM is in the \c st and 'false' otherwise
    bool isInState(QMState const * const st) const noexcept;

private:

    //! disallow inherited top() function in QP::QMsm and subclasses
    //! @sa QMsm::msm_top_s
    static QStateHandler top(
        void * const me,
        QEvt const * const e) noexcept = delete;

public:

    //! Return the current active state object (read only)
    QMState const * stateObj() const noexcept {
        return m_state.obj;
    }

    //! Obtain the current active child state of a given parent (read only)
    //!
    //! @details
    //! Finds the child state of the given `parent`, such that this child
    //! state is an ancestor of the currently active state. The main purpose
    //! of this function is to support **shallow history** transitions in
    //! state machines derived from QHsm.
    //!
    //! @param[in] parent pointer to the state-handler function
    //!
    //! @returns
    //! the child of a given `parent` state, which is an ancestor of the
    //! currently active state
    //!
    //! @note
    //! this function is designed to be called during state transitions, so it
    //! does not necessarily start in a stable state configuration.
    //! However, the function establishes stable state configuration upon exit.
    //!
    //! @tr{RQP103}
    //! @tr{RQP120H}
    QMState const * childStateObj(QMState const * const parent) const noexcept;

protected:

#ifdef Q_SPY
    //! Get the current state handler of the QMsm
    QStateHandler getStateHandler() noexcept override {
        return m_state.obj->stateHandler;
    }
#endif // def Q_SPY

private:

    //! Internal helper function to execute a transition-action table
    //!
    //! @details
    //! Helper function to execute transition sequence in a tran-action table.
    //!
    //! @param[in] tatbl pointer to the transition-action table
    //! @param[in] qs_id QS-id of this state machine (for QS local filter)
    //!
    //! @returns
    //! the status of the last action from the transition-action table.
    //!
    //! @note
    //! This function is for internal use inside the QEP event processor and
    //! should **not** be called directly from the applications.
    QState execTatbl_(
        QMTranActTable const * const tatbl,
        std::uint_fast8_t const qs_id);

    //! Internal helper function to exit current state to transition source
    //!
    //! @details
    //! Helper function to exit the current state configuration to the
    //! transition source, which is a hierarchical state machine might be a
    //! superstate of the current state.
    //!
    //! @param[in] s    pointer to the current state
    //! @param[in] ts   pointer to the transition source state
    //! @param[in] qs_id QS-id of this state machine (for QS local filter)
    void exitToTranSource_(
        QMState const * s,
        QMState const * const ts,
        std::uint_fast8_t const qs_id);

    //! Internal helper function to enter state history
    //!
    //! @details
    //! Static helper function to execute the segment of transition to history
    //! after entering the composite state and
    //!
    //! @param[in] hist pointer to the history substate
    //! @param[in] qs_id QS-id of this state machine (for QS local filter)
    //!
    //! @returns
    //! ::Q_RET_INIT, if an initial transition has been executed in the last
    //! entered state or ::Q_RET_NULL if no such transition was taken.
    QState enterHistory_(
        QMState const * const hist,
        std::uint_fast8_t const qs_id);
}; // class QMsm

} // namespace QP
//$enddecl${QEP} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//============================================================================
//$declare${QEP-macros} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${QEP-macros::Q_STATE_DECL} ................................................
//! Macro to generate a declaration of a state-handler, state-caller and
//! a state-object for a given state in a subclass of QP::QHsm.
#define Q_STATE_DECL(state_) \
    QP::QState state_ ## _h(QP::QEvt const * const e); \
    static QP::QState state_(void * const me, QP::QEvt const * const e)

//${QEP-macros::Q_STATE_DEF} .................................................
//! Macro to generate a declaration of a state-handler, state-caller and
//! a state-object for a given state in a subclass of QP::QHsm.
#define Q_STATE_DEF(subclass_, state_) \
    QP::QState subclass_::state_(void * const me, QP::QEvt const * const e) { \
        return static_cast<subclass_ *>(me)->state_ ## _h(e); } \
    QP::QState subclass_::state_ ## _h(QP::QEvt const * const e)

//${QEP-macros::Q_HANDLED} ...................................................
//! Macro to specify that the event was handled
#define Q_HANDLED() (Q_RET_HANDLED)

//${QEP-macros::Q_UNHANDLED} .................................................
//! Macro to specify that the event was NOT handled
//! due to a guard condition evaluating to 'false'
#define Q_UNHANDLED() (Q_RET_UNHANDLED)

//${QEP-macros::Q_EVT_CAST} ..................................................
//! Perform downcast of an event onto a subclass of QEvt `class_`
//!
//! @details
//! This macro encapsulates the downcast of QEvt pointers, which violates
//! MISRA-C 2004 rule 11.4(advisory). This macro helps to localize this
//! deviation.
#define Q_EVT_CAST(subclass_) (static_cast<subclass_ const *>(e))

//${QEP-macros::Q_STATE_CAST} ................................................
//! Macro to perform casting to QStateHandler.
//!
//! @details
//! This macro encapsulates the cast of a specific state handler function
//! pointer to QStateHandler, which violates MISRA-C 2004 rule 11.4(advisory).
//! This macro helps to localize this deviation.
#define Q_STATE_CAST(handler_) \
    (reinterpret_cast<QP::QStateHandler>(handler_))

//${QEP-macros::Q_ACTION_CAST} ...............................................
//! Macro to perform casting to QActionHandler.
//!
//! @details
//! This macro encapsulates the cast of a specific action handler function
//! pointer to QActionHandler, which violates MISRA-C2004 rule 11.4(advisory).
//! This macro helps to localize this deviation.
#define Q_ACTION_CAST(act_) \
    (reinterpret_cast<QP::QActionHandler>(act_))

//${QEP-macros::QM_STATE_DECL} ...............................................
//! Macro to generate a declaration of a state-handler, state-caller and
//! a state-object for a given state in a subclass of QP::QMsm.
#define QM_STATE_DECL(state_) \
    QP::QState state_ ## _h(QP::QEvt const * const e); \
    static QP::QState state_(void * const me, QP::QEvt const * const e); \
    static QP::QMState const state_ ## _s

//${QEP-macros::QM_SM_STATE_DECL} ............................................
//! Macro to generate a declaration of a state-handler, state-caller and
//! a state-object for a given *submachine* state in a subclass of QP::QMsm.
#define QM_SM_STATE_DECL(subm_, state_) \
    QP::QState state_ ## _h(QP::QEvt const * const e);\
    static QP::QState state_(void * const me, QP::QEvt const * const e); \
    static SM_ ## subm_ const state_ ## _s

//${QEP-macros::QM_ACTION_DECL} ..............................................
//! Macro to generate a declaration of an action-handler and action-caller
//! in a subclass of QP::QMsm.
#define QM_ACTION_DECL(action_) \
    QP::QState action_ ## _h(); \
    static QP::QState action_(void * const me)

//${QEP-macros::QM_STATE_DEF} ................................................
//! Macro to generate a definition of a state-caller and state-handler
//! for a given state in a subclass of QP::QMsm.
#define QM_STATE_DEF(subclass_, state_) \
    QP::QState subclass_::state_(void * const me, QP::QEvt const * const e) { \
        return static_cast<subclass_ *>(me)->state_ ## _h(e); } \
    QP::QState subclass_::state_ ## _h(QP::QEvt const * const e)

//${QEP-macros::QM_ACTION_DEF} ...............................................
//! Macro to generate a definition of an action-caller and action-handler
//! in a subclass of QP::QMsm.
#define QM_ACTION_DEF(subclass_, action_)  \
    QP::QState subclass_::action_(void * const me) { \
        return static_cast<subclass_ *>(me)->action_ ## _h(); } \
    QP::QState subclass_::action_ ## _h()

//${QEP-macros::QM_HANDLED} ..................................................
//! Macro for a QM action-handler when it handles the event.
#define QM_HANDLED() (Q_RET_HANDLED)

//${QEP-macros::QM_UNHANDLED} ................................................
//! Macro for a QM action-handler when it does not handle the event
//! due to a guard condition evaluating to false.
#define QM_UNHANDLED() (Q_RET_HANDLED)

//${QEP-macros::QM_SUPER} ....................................................
//! Macro for a QM action-handler when it passes the event to the superstate
#define QM_SUPER() (Q_RET_SUPER)

//${QEP-macros::QM_STATE_NULL} ...............................................
//! Macro to provide strictly-typed zero-state to use for submachines.
//! Applicable to suclasses of QP::QMsm.
#define QM_STATE_NULL (nullptr)

//${QEP-macros::Q_ACTION_NULL} ...............................................
//! Macro to provide strictly-typed zero-action to terminate action lists
//! in the transition-action-tables in QP::QMsm.
#define Q_ACTION_NULL (nullptr)

//${QEP-macros::Q_UNUSED_PAR} ................................................
//! Helper macro to clearly mark unused parameters of functions.
#define Q_UNUSED_PAR(par_) (static_cast<void>(par_))

//${QEP-macros::Q_DIM} .......................................................
//! Helper macro to calculate static dimension of a 1-dim `array_`
//!
//! @param[in] array_ 1-dimensional array
//!
//! @returns
//! the length of the array (number of elements it can hold)
#define Q_DIM(array_) (sizeof(array_) / sizeof((array_)[0U]))

//${QEP-macros::Q_UINT2PTR_CAST} .............................................
//! Perform cast from unsigned integer `uint_` to pointer of type `type_`
//!
//! @details
//! This macro encapsulates the cast to (type_ *), which QP ports or
//! application might use to access embedded hardware registers.
//! Such uses can trigger PC-Lint "Note 923: cast from int to pointer"
//! and this macro helps to encapsulate this deviation.
#define Q_UINT2PTR_CAST(type_, uint_) (reinterpret_cast<type_ *>(uint_))

//${QEP-macros::QEVT_INITIALIZER} ............................................
//! Initializer of static constant QEvt instances
//!
//! @details
//! This macro encapsulates the ugly casting of enumerated signals
//! to QSignal and constants for QEvt.poolID and QEvt.refCtr_.
#define QEVT_INITIALIZER(sig_) { static_cast<QP::QSignal>(sig_), 0U, 0U }
//$enddecl${QEP-macros} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#endif // QP_INC_QEP_HPP_
