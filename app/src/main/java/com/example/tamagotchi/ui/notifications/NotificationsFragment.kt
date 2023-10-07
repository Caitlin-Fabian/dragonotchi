package com.example.tamagotchi.ui.notifications
import androidx.compose.foundation.Image
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater

import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.material.Text
import androidx.compose.material3.Button

import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.ComposeView
import androidx.compose.ui.platform.LocalConfiguration
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import com.example.tamagotchi.R
import com.example.tamagotchi.databinding.FragmentNotificationsBinding

class NotificationsFragment : Fragment() {

    private var _binding: FragmentNotificationsBinding? = null

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {

        val notificationsViewModel =
            ViewModelProvider(this).get(NotificationsViewModel::class.java)

        _binding = FragmentNotificationsBinding.inflate(inflater, container, false)
        val root: View = binding.root

        val textView: TextView = binding.textNotifications
        notificationsViewModel.text.observe(viewLifecycleOwner) {
            textView.text = it

        }
        val rootView = inflater.inflate(R.layout.fragment_notifications, container, false)

        val composeView = rootView.findViewById<ComposeView>(R.id.composeView)

        composeView.setContent {
            // Define your Composable content here
            Text(
                text = notificationsViewModel.text.value ?: "Default Text",
                modifier = Modifier.padding(16.dp)
            )
            EggCrack()
        }


        return rootView
    }

    @Preview
    @Composable
    fun EggCrack(){
        val configuration = LocalConfiguration.current
        val screenHeight = configuration.screenHeightDp.dp
        val screenWidth = configuration.screenWidthDp.dp
            Column(modifier = Modifier.height(screenHeight).width(screenWidth).background(color = Color.White),
                verticalArrangement = Arrangement.Center,
                horizontalAlignment = Alignment.CenterHorizontally
            ){
            Text("Crack ")
                Button(onClick={(Log.d("debug", "Hello"))}){
                    Image(

                        modifier = Modifier.size(100.dp),
                        painter = painterResource(R.drawable.egg),
                        contentDescription = "Contact profile picture",
                    )
                }
            }
    }
    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}