//$file${src::qf::qep_hsm.cpp} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: qpcpp.qm
// File:  ${src::qf::qep_hsm.cpp}
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
//$endhead${src::qf::qep_hsm.cpp} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//! @file
//! @brief QP::QHsm implementation
//!
//! @tr{RQP103} @tr{RQP104} @tr{RQP120} @tr{RQP130}

#define QP_IMPL             // this is QP implementation
#include "qep_port.hpp"     // QEP port
#ifdef Q_SPY                // QS software tracing enabled?
    #include "qs_port.hpp"  // QS port
    #include "qs_pkg.hpp"   // QS facilities for pre-defined trace records
#else
    #include "qs_dummy.hpp" // disable the QS software tracing
#endif // Q_SPY
#include "qassert.h"        // QP embedded systems-friendly assertions

//============================================================================
//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 700U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpcpp version 7.0.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$define${QEP::versionStr[]} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace QP {

} // namespace QP
//$enddef${QEP::versionStr[]} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//============================================================================
// unnamed namespace for local definitions with internal linkage
namespace {
Q_DEFINE_THIS_MODULE("qep_hsm")

//----------------------------------------------------------------------------
//! Immutable events corresponding to the reserved signals.
//!
//! @details
//! Static, immutable reserved events that the QEP event processor sends
//! to state handler functions of QHsm-style state machine to execute entry
//! actions, exit actions, and initial transitions.
//!
static QP::QEvt const l_reservedEvt_[4] {
#ifdef Q_EVT_CTOR // Is the QEvt constructor provided?
    QP::QEvt(static_cast<QP::QSignal>(QP::QHsm::Q_EMPTY_SIG), 0U),
    QP::QEvt(static_cast<QP::QSignal>(QP::QHsm::Q_ENTRY_SIG), 0U),
    QP::QEvt(static_cast<QP::QSignal>(QP::QHsm::Q_EXIT_SIG),  0U),
    QP::QEvt(static_cast<QP::QSignal>(QP::QHsm::Q_INIT_SIG),  0U)
#else // QEvt is a POD (Plain Old Datatype)
    { static_cast<QP::QSignal>(QP::QHsm::Q_EMPTY_SIG), 0U, 0U },
    { static_cast<QP::QSignal>(QP::QHsm::Q_ENTRY_SIG), 0U, 0U },
    { static_cast<QP::QSignal>(QP::QHsm::Q_EXIT_SIG),  0U, 0U },
    { static_cast<QP::QSignal>(QP::QHsm::Q_INIT_SIG),  0U, 0U }
#endif
};

//----------------------------------------------------------------------------
// inline helper functions

//............................................................................
//! helper function to trigger reserved event in an QHsm
//!
//! @param[in] state state handler function
//! @param[in] sig   reserved signal to trigger
static inline QP::QState hsm_reservedEvt_(
    QP::QHsm * const me,
    QP::QStateHandler const state,
    enum QP::QHsm::ReservedSig const sig)
{
    return (*state)(me, &l_reservedEvt_[sig]);
}

//............................................................................
//! Helper function to execute entry into a given state in a
//! hierarchical state machine (HSM).
//!
//! @param[in] state   state handler function
//! @param[in] qs_id   QS-id of this state machine (for QS local filter)
static inline void hsm_state_entry_(
    QP::QHsm * const me,
    QP::QStateHandler const state,
    std::uint_fast8_t const qs_id)
{
#ifdef Q_SPY
    if ((*state)(me, &l_reservedEvt_[QP::QHsm::Q_ENTRY_SIG])
         == QP::QHsm::Q_RET_HANDLED)
    {
        QS_CRIT_STAT_
        QS_BEGIN_PRE_(QP::QS_QEP_STATE_ENTRY, qs_id)
            QS_OBJ_PRE_(me);
            QS_FUN_PRE_(state);
        QS_END_PRE_()
    }
#else
    Q_UNUSED_PAR(qs_id);
    static_cast<void>((*state)(me, &l_reservedEvt_[QP::QHsm::Q_ENTRY_SIG]));
#endif // Q_SPY
}

//............................................................................
//! Helper function to execute exit from a given state in a
//! hierarchical state machine (HSM).
//!
//! @param[in] state   state handler function
//! @param[in] qs_id   QS-id of this state machine (for QS local filter)
//!
//! @returns
//! 'true' if the exit action has been found in the state and
//! 'flase' otherwise.
static inline bool hsm_state_exit_(
    QP::QHsm * const me,
    QP::QStateHandler const state,
    std::uint_fast8_t const qs_id)
{
#ifdef Q_SPY
    bool isHandled;
    if ((*state)(me, &l_reservedEvt_[QP::QHsm::Q_EXIT_SIG])
        == QP::QHsm::Q_RET_HANDLED)
    {
        QS_CRIT_STAT_
        QS_BEGIN_PRE_(QP::QS_QEP_STATE_EXIT, qs_id)
            QS_OBJ_PRE_(me);
            QS_FUN_PRE_(state);
        QS_END_PRE_()
        isHandled = true;
    }
    else {
        isHandled = false;
    }
    return isHandled;
#else
    Q_UNUSED_PAR(qs_id);
    return (*state)(me, &l_reservedEvt_[QP::QHsm::Q_EXIT_SIG]);
#endif // Q_SPY
}

} // unnamed namespace

//$define${QEP::QHsm} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace QP {

//${QEP::QHsm} ...............................................................

//${QEP::QHsm::QHsm} .........................................................
QHsm::QHsm(QStateHandler const initial) noexcept{
    m_state.fun = Q_STATE_CAST(&top);
    m_temp.fun  = initial;
}

//${QEP::QHsm::init} .........................................................
void QHsm::init(
    void const * const e,
    std::uint_fast8_t const qs_id)
{
    #ifdef Q_SPY
    if ((QS::priv_.flags & 0x01U) == 0U) {
        QS::priv_.flags |= 0x01U;
        QS_FUN_DICTIONARY(&QP::QHsm::top);
    }
    #else
    Q_UNUSED_PAR(qs_id);
    #endif

    QStateHandler t = m_state.fun;

    //! @pre ctor must have been executed and initial tran NOT taken
    Q_REQUIRE_ID(200, (m_temp.fun != nullptr)
                      && (t == Q_STATE_CAST(&top)));

    // execute the top-most initial transition
    QState r = (*m_temp.fun)(this, Q_EVT_CAST(QEvt));

    // the top-most initial transition must be taken
    Q_ASSERT_ID(210, r == Q_RET_TRAN);

    QS_CRIT_STAT_
    QS_BEGIN_PRE_(QS_QEP_STATE_INIT, qs_id)
        QS_OBJ_PRE_(this);       // this state machine object
        QS_FUN_PRE_(t);          // the source state
        QS_FUN_PRE_(m_temp.fun); // the target of the initial transition
    QS_END_PRE_()

    // drill down into the state hierarchy with initial transitions...
    do {
        QStateHandler path[MAX_NEST_DEPTH_]; // tran entry path array
        std::int_fast8_t ip = 0; // entry path index

        path[0] = m_temp.fun;
        static_cast<void>(hsm_reservedEvt_(this, m_temp.fun, Q_EMPTY_SIG));
        while (m_temp.fun != t) {
            ++ip;
            Q_ASSERT_ID(220, ip < MAX_NEST_DEPTH_);
            path[ip] = m_temp.fun;
            static_cast<void>(hsm_reservedEvt_(this, m_temp.fun, Q_EMPTY_SIG));
        }
        m_temp.fun = path[0];

        // retrace the entry path in reverse (desired) order...
        do {
            hsm_state_entry_(this, path[ip], qs_id); // enter path[ip]
            --ip;
        } while (ip >= 0);

        t = path[0]; // current state becomes the new source

        r = hsm_reservedEvt_(this, t, Q_INIT_SIG); // execute initial tran.

    #ifdef Q_SPY
        if (r == Q_RET_TRAN) {
            QS_BEGIN_PRE_(QS_QEP_STATE_INIT, qs_id)
                QS_OBJ_PRE_(this);       // this state machine object
                QS_FUN_PRE_(t);          // the source state
                QS_FUN_PRE_(m_temp.fun); // the target of the initial tran.
            QS_END_PRE_()
        }
    #endif // Q_SPY

    } while (r == Q_RET_TRAN);

    QS_BEGIN_PRE_(QS_QEP_INIT_TRAN, qs_id)
        QS_TIME_PRE_();    // time stamp
        QS_OBJ_PRE_(this); // this state machine object
        QS_FUN_PRE_(t);    // the new active state
    QS_END_PRE_()

    m_state.fun = t; // change the current active state
    m_temp.fun  = t; // mark the configuration as stable
}

//${QEP::QHsm::dispatch} .....................................................
void QHsm::dispatch(
    QEvt const * const e,
    std::uint_fast8_t const qs_id)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qs_id);
    #endif

    QStateHandler t = m_state.fun;
    QS_CRIT_STAT_

    //! @pre the current state must be initialized and
    //! the state configuration must be stable
    Q_REQUIRE_ID(400, (t != nullptr)
                       && (t == m_temp.fun));

    QS_BEGIN_PRE_(QS_QEP_DISPATCH, qs_id)
        QS_TIME_PRE_();         // time stamp
        QS_SIG_PRE_(e->sig);    // the signal of the event
        QS_OBJ_PRE_(this);      // this state machine object
        QS_FUN_PRE_(t);         // the current state
    QS_END_PRE_()

    QStateHandler s;
    QState r;
    // process the event hierarchically...
    //! @tr{RQP120A}
    do {
        s = m_temp.fun;
        r = (*s)(this, e); // invoke state handler s

        if (r == Q_RET_UNHANDLED) { // unhandled due to a guard?

            QS_BEGIN_PRE_(QS_QEP_UNHANDLED, qs_id)
                QS_SIG_PRE_(e->sig); // the signal of the event
                QS_OBJ_PRE_(this);   // this state machine object
                QS_FUN_PRE_(s);      // the current state
            QS_END_PRE_()

            r = hsm_reservedEvt_(this, s, Q_EMPTY_SIG); // find superstate of s
        }
    } while (r == Q_RET_SUPER);

    // regular transition taken?
    //! @tr{RQP120E}
    if (r >= Q_RET_TRAN) {
        QStateHandler path[MAX_NEST_DEPTH_];

        path[0] = m_temp.fun; // save the target of the transition
        path[1] = t;
        path[2] = s;

        // exit current state to transition source s...
        //! @tr{RQP120C}
        for (; t != s; t = m_temp.fun) {
            // exit handled?
            if (hsm_state_exit_(this, t, qs_id)) {
                // find superstate of t
                static_cast<void>(hsm_reservedEvt_(this, t, Q_EMPTY_SIG));
            }
        }

        std::int_fast8_t ip = hsm_tran(path, qs_id); // the HSM transition

    #ifdef Q_SPY
        if (r == Q_RET_TRAN_HIST) {

            QS_BEGIN_PRE_(QS_QEP_TRAN_HIST, qs_id)
                QS_OBJ_PRE_(this);     // this state machine object
                QS_FUN_PRE_(t);        // the source of the transition
                QS_FUN_PRE_(path[0]);  // the target of the tran. to history
            QS_END_PRE_()

        }
    #endif // Q_SPY

        // execute state entry actions in the desired order...
        //! @tr{RQP120B}
        for (; ip >= 0; --ip) {
            hsm_state_entry_(this, path[ip], qs_id); // enter path[ip]
        }
        t = path[0];    // stick the target into register
        m_temp.fun = t; // update the next state

        // drill into the target hierarchy...
        //! @tr{RQP120I}
        while (hsm_reservedEvt_(this, t, Q_INIT_SIG) == Q_RET_TRAN) {

            QS_BEGIN_PRE_(QS_QEP_STATE_INIT, qs_id)
                QS_OBJ_PRE_(this);       // this state machine object
                QS_FUN_PRE_(t);          // the source (pseudo)state
                QS_FUN_PRE_(m_temp.fun); // the target of the transition
            QS_END_PRE_()

            ip = 0;
            path[0] = m_temp.fun;

            // find superstate
            static_cast<void>(hsm_reservedEvt_(this, m_temp.fun, Q_EMPTY_SIG));

            while (m_temp.fun != t) {
                ++ip;
                path[ip] = m_temp.fun;
                // find superstate
                static_cast<void>(
                    hsm_reservedEvt_(this, m_temp.fun, Q_EMPTY_SIG));
            }
            m_temp.fun = path[0];

            // entry path must not overflow
            Q_ASSERT_ID(410, ip < MAX_NEST_DEPTH_);

            // retrace the entry path in reverse (correct) order...
            do {
                hsm_state_entry_(this, path[ip], qs_id);  // enter path[ip]
                --ip;
            } while (ip >= 0);

            t = path[0];
        }

        QS_BEGIN_PRE_(QS_QEP_TRAN, qs_id)
            QS_TIME_PRE_();          // time stamp
            QS_SIG_PRE_(e->sig);     // the signal of the event
            QS_OBJ_PRE_(this);       // this state machine object
            QS_FUN_PRE_(s);          // the source of the transition
            QS_FUN_PRE_(t);          // the new active state
        QS_END_PRE_()
    }

    #ifdef Q_SPY
    else if (r == Q_RET_HANDLED) {

        QS_BEGIN_PRE_(QS_QEP_INTERN_TRAN, qs_id)
            QS_TIME_PRE_();          // time stamp
            QS_SIG_PRE_(e->sig);     // the signal of the event
            QS_OBJ_PRE_(this);       // this state machine object
            QS_FUN_PRE_(s);          // the source state
        QS_END_PRE_()

    }
    else {

        QS_BEGIN_PRE_(QS_QEP_IGNORED, qs_id)
            QS_TIME_PRE_();          // time stamp
            QS_SIG_PRE_(e->sig);     // the signal of the event
            QS_OBJ_PRE_(this);       // this state machine object
            QS_FUN_PRE_(m_state.fun);// the current state
        QS_END_PRE_()

    }
    #else
        Q_UNUSED_PAR(qs_id); // when Q_SPY not defined
    #endif // Q_SPY

    m_state.fun = t; // change the current active state
    m_temp.fun  = t; // mark the configuration as stable
}

//${QEP::QHsm::top} ..........................................................
QState QHsm::top(
    void * const me,
    QEvt const * const e) noexcept
{
    Q_UNUSED_PAR(me);
    Q_UNUSED_PAR(e);
    return Q_RET_IGNORED; // the top state ignores all events
}

//${QEP::QHsm::isIn} .........................................................
bool QHsm::isIn(QStateHandler const s) noexcept {
    //! @pre state configuration must be stable
    Q_REQUIRE_ID(600, m_temp.fun == m_state.fun);

    bool inState = false;  // assume that this HSM is not in 'state'

    // scan the state hierarchy bottom-up
    QState r;
    do {
        // do the states match?
        if (m_temp.fun == s) {
            inState = true;    // 'true' means that match found
            r = Q_RET_IGNORED; // cause breaking out of the loop
        }
        else {
            r = hsm_reservedEvt_(this, m_temp.fun, Q_EMPTY_SIG);
        }
    } while (r != Q_RET_IGNORED); // QHsm::top() state not reached
    m_temp.fun = m_state.fun; // restore the stable state configuration

    return inState; // return the status
}

//${QEP::QHsm::childState} ...................................................
QStateHandler QHsm::childState(QStateHandler const parent) noexcept {
    QStateHandler child = m_state.fun; // start with the current state
    bool isFound = false; // start with the child not found

    // establish stable state configuration
    m_temp.fun = m_state.fun;
    QState r;
    do {
        // is this the parent of the current child?
        if (m_temp.fun == parent) {
            isFound = true; // child is found
            r = Q_RET_IGNORED;  // cause breaking out of the loop
        }
        else {
            child = m_temp.fun;
            r = hsm_reservedEvt_(this, m_temp.fun, Q_EMPTY_SIG);
        }
    } while (r != Q_RET_IGNORED); // QHsm::top() state not reached
    m_temp.fun = m_state.fun; // establish stable state configuration

    //! @post the child must be confirmed
    Q_ENSURE_ID(810, isFound);

    #ifdef Q_NASSERT
    Q_UNUSED_PAR(isFound);
    #endif

    return child; // return the child
}

//${QEP::QHsm::hsm_tran} .....................................................
std::int_fast8_t QHsm::hsm_tran(
    QStateHandler (&path)[MAX_NEST_DEPTH_],
    std::uint_fast8_t const qs_id)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qs_id);
    #endif

    std::int_fast8_t ip = -1; // transition entry path index
    QStateHandler t = path[0];
    QStateHandler const s = path[2];

    // (a) check source==target (transition to self)
    if (s == t) {
        // exit the source
        static_cast<void>(hsm_state_exit_(this, s, qs_id));
        ip = 0; // enter the target
    }
    else {
        // superstate of target
        static_cast<void>(hsm_reservedEvt_(this, t, Q_EMPTY_SIG));
        t = m_temp.fun;

        // (b) check source==target->super
        if (s == t) {
            ip = 0; // enter the target
        }
        else {
            // superstate of src
            static_cast<void>(hsm_reservedEvt_(this, s, Q_EMPTY_SIG));

            // (c) check source->super==target->super
            if (m_temp.fun == t) {
                // exit the source
                static_cast<void>(hsm_state_exit_(this, s, qs_id));
                ip = 0; // enter the target
            }
            else {
                // (d) check source->super==target
                if (m_temp.fun == path[0]) {
                    // exit the source
                    static_cast<void>(hsm_state_exit_(this, s, qs_id));
                }
                else {
                    // (e) check rest of source==target->super->super..
                    // and store the entry path along the way
                    std::int_fast8_t iq = 0; // indicate that LCA was found
                    ip = 1; // enter target and its superstate
                    path[1] = t; // save the superstate of target
                    t = m_temp.fun; // save source->super

                    // find target->super->super
                    QState r = hsm_reservedEvt_(this, path[1], Q_EMPTY_SIG);
                    while (r == Q_RET_SUPER) {
                        ++ip;
                        path[ip] = m_temp.fun; // store the entry path
                        if (m_temp.fun == s) { // is it the source?
                            // indicate that the LCA was found
                            iq = 1;

                            // entry path must not overflow
                            Q_ASSERT_ID(510, ip < MAX_NEST_DEPTH_);
                            --ip;  // do not enter the source
                            r = Q_RET_HANDLED; // terminate the loop
                        }
                        // it is not the source, keep going up
                        else {
                            r = hsm_reservedEvt_(this, m_temp.fun, Q_EMPTY_SIG);
                        }
                    }

                    // the LCA not found yet?
                    if (iq == 0) {
                        // entry path must not overflow
                        Q_ASSERT_ID(520, ip < MAX_NEST_DEPTH_);

                        // exit the source
                        static_cast<void>(hsm_state_exit_(this, s, qs_id));

                        // (f) check the rest of source->super
                        //                  == target->super->super...
                        //
                        iq = ip;
                        r = Q_RET_IGNORED; // indicate LCA NOT found
                        do {
                            // is this the LCA?
                            if (t == path[iq]) {
                                r = Q_RET_HANDLED; // indicate LCA found
                                ip = iq - 1; // do not enter LCA
                                iq = -1; // cause termination of the loop
                            }
                            else {
                                --iq; // try lower superstate of target
                            }
                        } while (iq >= 0);

                        // LCA not found yet?
                        if (r != Q_RET_HANDLED) {
                            // (g) check each source->super->...
                            // for each target->super...
                            //
                            r = Q_RET_IGNORED; // keep looping
                            do {
                                // exit from t handled?
                                if (hsm_state_exit_(this, t, qs_id)) {
                                    // find superstate of t
                                    static_cast<void>(
                                        hsm_reservedEvt_(this, t, Q_EMPTY_SIG));
                                }
                                t = m_temp.fun; //  set to super of t
                                iq = ip;
                                do {
                                    // is this LCA?
                                    if (t == path[iq]) {
                                        ip = iq - 1; // do not enter LCA
                                        iq = -1; // break out of inner loop
                                        r = Q_RET_HANDLED; // break outer loop
                                    }
                                    else {
                                        --iq;
                                    }
                                } while (iq >= 0);
                            } while (r != Q_RET_HANDLED);
                        }
                    }
                }
            }
        }
    }
    return ip;

}

} // namespace QP
//$enddef${QEP::QHsm} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
